#include <iostream>
using namespace std;

const int sz = 10000;

struct s_t {
    long long cmp;
    long long wrt;
    long long mrg;
    int dep;
};

void r(s_t& s) {
    s.cmp = 0;
    s.wrt = 0;
    s.mrg = 0;
    s.dep = 0;
}

void cp(const int src[], int dst[], int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

void pt(const int a[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << a[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "]";
}

bool chk(const int a[], int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) return false;
    }
    return true;
}

long long sm(const int a[], int n) {
    long long t = 0;
    for (int i = 0; i < n; i++) t += a[i];
    return t;
}

long long sq(const int a[], int n) {
    long long t = 0;
    for (int i = 0; i < n; i++) t += 1LL * a[i] * a[i];
    return t;
}

bool eq(const int a[], const int b[], int n) {
    return sm(a, n) == sm(b, n) && sq(a, n) == sq(b, n);
}

void mg(int a[], int tmp[], int l, int m, int r, s_t& s, bool tr, int d) {
    int i = l;
    int j = m + 1;
    int k = l;
    s.mrg++;

    if (tr) {
        cout << "Depth " << d << ", merge [" << l << ".." << m << "] and [" << m + 1 << ".." << r << "]\n";
        cout << "Left part: ";
        pt(a + l, m - l + 1);
        cout << "\nRight part: ";
        pt(a + m + 1, r - m);
        cout << "\n";
    }

    while (i <= m && j <= r) {
        s.cmp++;
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
        s.wrt++;
    }

    while (i <= m) {
        tmp[k++] = a[i++];
        s.wrt++;
    }

    while (j <= r) {
        tmp[k++] = a[j++];
        s.wrt++;
    }

    for (int p = l; p <= r; p++) {
        a[p] = tmp[p];
        s.wrt++;
    }

    if (tr) {
        cout << "Merged result: ";
        pt(a + l, r - l + 1);
        cout << "\n";
    }
}

void ms_rec(int a[], int tmp[], int l, int r, s_t& s, int d, bool tr) {
    if (d > s.dep) s.dep = d;
    if (l >= r) return;
    int m = l + (r - l) / 2;
    if (tr) {
        cout << "Depth " << d << ", split [" << l << ".." << r << "] at " << m << "\n";
    }
    ms_rec(a, tmp, l, m, s, d + 1, tr);
    ms_rec(a, tmp, m + 1, r, s, d + 1, tr);
    mg(a, tmp, l, m, r, s, tr, d);
}

void ms(int a[], int n, s_t& s, bool tr = false) {
    r(s);
    int tmp[sz];
    if (n > 0) ms_rec(a, tmp, 0, n - 1, s, 1, tr);
}

void ins(int a[], int n, s_t& s) {
    r(s);
    for (int i = 1; i < n; i++) {
        int v = a[i];
        int j = i - 1;
        while (j >= 0) {
            s.cmp++;
            if (a[j] > v) {
                a[j + 1] = a[j];
                s.wrt++;
                j--;
            }
            else break;
        }
        a[j + 1] = v;
        s.wrt++;
    }
}

void sel(int a[], int n, s_t& s) {
    r(s);
    for (int i = 0; i < n - 1; i++) {
        int mn = i;
        for (int j = i + 1; j < n; j++) {
            s.cmp++;
            if (a[j] < a[mn]) mn = j;
        }
        if (mn != i) {
            int tmp = a[i]; a[i] = a[mn]; a[mn] = tmp;
            s.wrt += 3;
        }
    }
}

void bub(int a[], int n, s_t& s) {
    r(s);
    for (int i = 0; i < n - 1; i++) {
        bool ch = false;
        for (int j = 0; j < n - 1 - i; j++) {
            s.cmp++;
            if (a[j] > a[j + 1]) {
                int tmp = a[j]; a[j] = a[j + 1]; a[j + 1] = tmp;
                s.wrt += 3;
                ch = true;
            }
        }
        if (!ch) break;
    }
}

void shl(int a[], int n, s_t& s) {
    r(s);
    for (int g = n / 2; g > 0; g /= 2) {
        for (int i = g; i < n; i++) {
            int v = a[i];
            int j = i;
            while (j >= g) {
                s.cmp++;
                if (a[j - g] > v) {
                    a[j] = a[j - g];
                    s.wrt++;
                    j -= g;
                }
                else break;
            }
            a[j] = v;
            s.wrt++;
        }
    }
}

void hfy(int a[], int n, int i, s_t& s) {
    int lg = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n) {
        s.cmp++;
        if (a[l] > a[lg]) lg = l;
    }
    if (r < n) {
        s.cmp++;
        if (a[r] > a[lg]) lg = r;
    }
    if (lg != i) {
        int tmp = a[i]; a[i] = a[lg]; a[lg] = tmp;
        s.wrt += 3;
        hfy(a, n, lg, s);
    }
}

void hps(int a[], int n, s_t& s) {
    r(s);
    for (int i = n / 2 - 1; i >= 0; i--) hfy(a, n, i, s);
    for (int end = n - 1; end > 0; end--) {
        int tmp = a[0]; a[0] = a[end]; a[end] = tmp;
        s.wrt += 3;
        hfy(a, end, 0, s);
    }
}

int prt(int a[], int low, int high, s_t& s) {
    int mid = low + (high - low) / 2;
    if (mid != high) {
        int tmp = a[mid]; a[mid] = a[high]; a[high] = tmp;
        s.wrt += 3;
    }
    int pv = a[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        s.cmp++;
        if (a[j] <= pv) {
            i++;
            if (i != j) {
                int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
                s.wrt += 3;
            }
        }
    }
    if (i + 1 != high) {
        int tmp = a[i + 1]; a[i + 1] = a[high]; a[high] = tmp;
        s.wrt += 3;
    }
    return i + 1;
}

void qsr(int a[], int low, int high, s_t& s, int d) {
    if (d > s.dep) s.dep = d;
    if (low < high) {
        int p = prt(a, low, high, s);
        qsr(a, low, p - 1, s, d + 1);
        qsr(a, p + 1, high, s, d + 1);
    }
}

void qck(int a[], int n, s_t& s) {
    r(s);
    if (n > 0) qsr(a, 0, n - 1, s, 1);
}

bool tm_t(const int lt[], int nl, const int rt[], int nr, const int exp[], int no) {
    int a[sz], tmp[sz];
    int n = nl + nr;
    for (int i = 0; i < nl; i++) a[i] = lt[i];
    for (int i = 0; i < nr; i++) a[nl + i] = rt[i];
    s_t s;
    r(s);
    mg(a, tmp, 0, nl - 1, n - 1, s, false, 1);
    bool ok = chk(a, n);
    for (int i = 0; i < n && ok; i++) {
        if (a[i] != exp[i]) ok = false;
    }
    if (n == 0) ok = true;
    cout << "Merge test " << no << ": " << (ok ? "OK" : "ERROR") << "\n";
    return ok;
}

bool tms_t(const int inp[], int n, const int exp[], int no) {
    int a[sz], org[sz];
    cp(inp, a, n);
    cp(inp, org, n);
    s_t s;
    ms(a, n, s);
    bool ok = chk(a, n) && eq(org, a, n);
    for (int i = 0; i < n && ok; i++) {
        if (a[i] != exp[i]) ok = false;
    }
    cout << "MergeSort test " << no << ": " << (ok ? "OK" : "ERROR") << "\n";
    return ok;
}

void run(const char* nm, const int src[], int n, int mtd) {
    int a[sz];
    cp(src, a, n);
    s_t s;
    if (mtd == 0) ins(a, n, s);
    else if (mtd == 1) sel(a, n, s);
    else if (mtd == 2) bub(a, n, s);
    else if (mtd == 3) shl(a, n, s);
    else if (mtd == 4) qck(a, n, s);
    else if (mtd == 5) hps(a, n, s);
    else ms(a, n, s);

    cout << nm << "; " << s.cmp << "; " << s.wrt
        << "; " << s.dep << "; "
        << (chk(a, n) && eq(src, a, n) ? "OK" : "ERROR")
        << "\n";
}


void gen_data(int a[], int n, int z) {
    for (int i = 0; i < n; i++) {
        if (z == 0) a[i] = i + 1;                    
        else if (z == 1) a[i] = n - i;                
        else if (z == 2) a[i] = i;                    
        else a[i] = (i * 37 + 11) % (n + 17);        
    }
}


void exp_table() {
    const int sizes[] = { 100, 1000, 10000 };
    const char* types[] = { "ascending", "descending", "nearly_sorted", "mixed" };
    const char* algs[] = { "Insertion", "Selection", "Bubble", "Shell", "QuickSort", "HeapSort", "MergeSort" };

    cout << "\nCOMPARATIVE EXPERIMENT TABLES (100, 1000, 10000)\n";
    cout << "size; type; algorithm; comparisons; writes; depth; check\n";

    for (int sz_idx = 0; sz_idx < 3; sz_idx++) {
        int n = sizes[sz_idx];
        for (int t_idx = 0; t_idx < 4; t_idx++) {
            int a[10000];
            gen_data(a, n, t_idx);
            if (t_idx == 2) { // Nearly sorted adjustments
                for (int i = 0; i < n; i += 10) {
                    int tmp = a[i]; a[i] = a[(i + 1) % n]; a[(i + 1) % n] = tmp;
                }
            }
            for (int alg = 0; alg < 7; alg++) {
                int work[10000];
                cp(a, work, n);
                s_t s;
                int org[10000];
                cp(a, org, n);

                if (alg == 0) ins(work, n, s);
                else if (alg == 1) sel(work, n, s);
                else if (alg == 2) bub(work, n, s);
                else if (alg == 3) shl(work, n, s);
                else if (alg == 4) qck(work, n, s);
                else if (alg == 5) hps(work, n, s);
                else ms(work, n, s);

                cout << n << "; " << types[t_idx] << "; " << algs[alg] << "; "
                    << s.cmp << "; " << s.wrt << "; " << s.dep << "; "
                    << (chk(work, n) && eq(org, work, n) ? "OK" : "ERROR") << "\n";
            }
        }
    }
}

int main() {
    cout << "PRACTICAL WORK 7. MERGESORT\n\n";
    cout << "VARIANT 5\n";

    const int v[] = {
        23, 2, 28, 11, 6, 30, 9, 19,
        4, 26, 14, 7, 29, 1, 17, 22
    };
    const int n = 16;
    int a[sz], org[sz];
    cp(v, a, n);
    cp(v, org, n);

    cout << "Initial array: ";
    pt(a, n);
    cout << "\n\nMERGESORT TRACE\n";
    s_t mst;
    ms(a, n, mst, true);

    cout << "\nSorted array: ";
    pt(a, n);
    cout << "\nComparisons: " << mst.cmp
        << "\nWrites: " << mst.wrt
        << "\nMerges: " << mst.mrg
        << "\nMaximum recursion depth: " << mst.dep
        << "\nOrder and content check: "
        << (chk(a, n) && eq(org, a, n) ? "OK" : "ERROR")
        << "\n\n";

    const int l1[] = { 1, 3 };
    const int r1[] = { 2, 4 };
    const int e1[] = { 1, 2, 3, 4 };

    const int l2[] = { 1, 2, 3 };
    const int r2[] = { 4 };
    const int e2[] = { 1, 2, 3, 4 };

    const int l3[] = { 5, 7 };
    const int r3[] = { 1, 2 };
    const int e3[] = { 1, 2, 5, 7 };

    const int l4[] = { 1, 2, 2 };
    const int r4[] = { 2, 3 };
    const int e4[] = { 1, 2, 2, 2, 3 };

    const int l5[] = { -5, -1 };
    const int r5[] = { -3, 0 };
    const int e5[] = { -5, -3, -1, 0 };

    const int* l6 = nullptr;
    const int r6[] = { 1, 2, 3, 4 };
    const int e6[] = { 1, 2, 3, 4 };

    int mp_cnt = 0;
    mp_cnt += tm_t(l1, 2, r1, 2, e1, 1);
    mp_cnt += tm_t(l2, 3, r2, 1, e2, 2);
    mp_cnt += tm_t(l3, 2, r3, 2, e3, 3);
    mp_cnt += tm_t(l4, 3, r4, 2, e4, 4);
    mp_cnt += tm_t(l5, 2, r5, 2, e5, 5);
    mp_cnt += tm_t(l6, 0, r6, 4, e6, 6);
    cout << "Successful merge tests: " << mp_cnt << "/6\n\n";

    const int* t1 = nullptr;
    const int* e0 = nullptr;
    const int t2[] = { 7 };
    const int e2s[] = { 7 };
    const int t3[] = { 1, 2 };
    const int e3s[] = { 1, 2 };
    const int t4[] = { 2, 1 };
    const int e4s[] = { 1, 2 };
    const int t5[] = { 1, 2, 3, 4, 5 };
    const int e5s[] = { 1, 2, 3, 4, 5 };
    const int t6[] = { 5, 4, 3, 2, 1 };
    const int e6s[] = { 1, 2, 3, 4, 5 };
    const int t7[] = { 4, 1, 5, 2, 3 };
    const int e7s[] = { 1, 2, 3, 4, 5 };
    const int t8[] = { 3, 1, 3, 2, 1 };
    const int e8s[] = { 1, 1, 2, 3, 3 };
    const int t9[] = { 0, -5, 8, -2, 4 };
    const int e9s[] = { -5, -2, 0, 4, 8 };
    const int t10[] = { 6, 6, 6, 6 };
    const int e10s[] = { 6, 6, 6, 6 };
    const int t11[] = { 9, -1, 9, 0, -1, 5 };
    const int e11s[] = { -1, -1, 0, 5, 9, 9 };
    const int t12[] = { 8, 3, 7, 2, 6, 1, 5 };
    const int e12s[] = { 1, 2, 3, 5, 6, 7, 8 };
    const int t13[] = { 12, 4, 7, 4, 0, -3, 8, 2, 1, 9, 5, 6, 3, 11, 10 };
    const int e13s[] = { -3, 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    const int t14[] = { 2147483647, 0, -2147483647, 4 };
    const int e14s[] = { -2147483647, 0, 4, 2147483647 };

    int msp_cnt = 0;
    msp_cnt += tms_t(t1, 0, e0, 1);
    msp_cnt += tms_t(t2, 1, e2s, 2);
    msp_cnt += tms_t(t3, 2, e3s, 3);
    msp_cnt += tms_t(t4, 2, e4s, 4);
    msp_cnt += tms_t(t5, 5, e5s, 5);
    msp_cnt += tms_t(t6, 5, e6s, 6);
    msp_cnt += tms_t(t7, 5, e7s, 7);
    msp_cnt += tms_t(t8, 5, e8s, 8);
    msp_cnt += tms_t(t9, 5, e9s, 9);
    msp_cnt += tms_t(t10, 4, e10s, 10);
    msp_cnt += tms_t(t11, 6, e11s, 11);
    msp_cnt += tms_t(t12, 7, e12s, 12);
    msp_cnt += tms_t(t13, 15, e13s, 13);
    msp_cnt += tms_t(t14, 4, e14s, 14);
    cout << "Successful MergeSort tests: " << msp_cnt << "/14\n\n";

    cout << "COMPARISON ON VARIANT 5\n";
    cout << "Algorithm; comparisons; writes/swaps; max depth; check\n";
    run("Insertion", v, n, 0);
    run("Selection", v, n, 1);
    run("Bubble", v, n, 2);
    run("Shell", v, n, 3);
    run("QuickSort", v, n, 4);
    run("HeapSort", v, n, 5);
    run("MergeSort", v, n, 6);

    
    exp_table();

    return 0;
}
