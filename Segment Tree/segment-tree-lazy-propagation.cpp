#include<bits/stdc++.h>
using namespace std;

class ST {
	vector<int> st, lazy;
public:
	ST(int n ) {
		st.resize(4 * n);
		lazy.resize(4 * n);
	}
	void build(int idx, int low, int high, int arr[]) {
		if (low == high) {
			st[idx] = arr[low];
			return;
		}
		int mid = (low + high) >> 1;
		build(2 * idx + 1, low, mid, arr);
		build(2 * idx + 2, mid + 1, high, arr);
		st[idx] = st[2 * idx + 1] + st[2 * idx + 2];
	}
	void update(int idx, int low, int high, int l, int r, int val) {
		// update prev remaining updates and propagate downwards
		if (lazy[idx] != 0) {
			// no of nodes the current node is responsible for -> (high-low+1)
			st[idx] += (high - low + 1) * lazy[idx];
			// propagate downwards
			if (low != high) { //child present
				lazy[2 * idx + 1] += lazy[idx];
				lazy[2 * idx + 2] += lazy[idx];
			}
			lazy[idx] = 0;
		}

		// no overlap -> low high l r OR l r low high
		if (high < l or r < low)	return;

		// complete overlap -> l low high r
		if (low >= l && high <= r) {
			st[idx] += (high - low + 1) * val;
			if (low != high) {
				lazy[2 * idx + 1] += val;
				lazy[2 * idx + 2] += val;
			}
			return;
		}
		// partial
		int mid = (low + high) >> 1;
		update(2 * idx + 1, low, mid, l, r, val);
		update(2 * idx + 2, mid + 1, high, l, r, val);
		st[idx] = st[2 * idx + 1] + st[2 * idx + 2];
	}
	int query(int idx, int low, int high, int l, int r) {

		// update if any updates remaining
		if (lazy[idx] != 0) {
			// no of nodes the current node is responsible for -> (high-low+1)
			st[idx] += (high - low + 1) * lazy[idx];
			// propagate downwards
			if (low != high) { //child present
				lazy[2 * idx + 1] += lazy[idx];
				lazy[2 * idx + 2] += lazy[idx];
			}
			lazy[idx] = 0;
		}

		// no overlap -> low high l r OR l r low high
		if (high < l or r < low)	return 0;

		// complete overlap -> l low high r
		if (low >= l && high <= r)	return st[idx];

		int mid = (low + high) >> 1;
		int left = query(2 * idx + 1, low, mid, l, r);
		int right = query(2 * idx + 2, mid + 1, high, l, r);
		return left + right;
	}
};


int main() {
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif
	int n;
	cin >> n;
	int arr[n];
	for (int i = 0; i < n; i++) cin >> arr[i];
	ST st(n);

	st.build(0, 0, n - 1, arr);

	int q; cin >> q;
	while (q--) {
		int type; cin >> type;
		if (type == 1) {
			int l, r;
			cin >> l >> r;
			cout << st.query(0, 0, n - 1, l, r) << endl;
		}
		else {
			int l, r, val;
			cin >> l >> r >> val;
			st.update(0, 0, n - 1, l, r, val);
		}
	}
	return 0;
}
