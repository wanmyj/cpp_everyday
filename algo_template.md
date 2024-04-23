- [二分查找](#二分查找)
- [链表题目](#链表题目)
- [二叉树](#二叉树)
- [动态规划](#动态规划)
- [背包问题](#背包问题)
  - [01](#01)
  - [完全背包](#完全背包)
  - [能不能凑成target](#能不能凑成target)
  - [到底有多少种组合](#到底有多少种组合)
- [堆排序](#堆排序)
- [快速排序](#快速排序)


## 二分查找
模板: 最后做个start检查

```cpp
while(start < end) {
    int mid = start + (end - start) / 2;
    if (nums[mid] == target) {
        return mid;
    } else if (nums[mid] > target) {
        right = mid - 1;
    } else if (nums[mid] < target) {
        start = mid + 1;
    }
}
if (nums[start] == target) return start;
return -1;
```

有重复：
```cpp
int searchRange(const vector<int>& nums, int target) {
    int n = nums.size();
    if (!n) return {-1};
    int start = 0, end = n - 1;
    int res_start = -1, res_end = -1;
    // last index
    while (start + 1 < end)
    {
        int mid = start + (end -start) / 2;
        if (nums[mid] == target) {
            start = mid; //NOTE HERE!
        } else if (nums[mid] > target) {
            end = mid;
        } else if (nums[mid] < target) {
            start = mid;
        }
    }
    if (nums[end] == target) {
        return end;
    } else if (nums[start] == target) {
        return start;
    } else {
        return {-1};
    }
}
```
while要使用 start+1 < end\
出来的时候是两个数，一个是end，一个是start\
等于的时候不能直接return

第一个元素：\
等于的时候把mid给end（避免mid+-1）
先判断start，再判断end\
最后一个元素：\
等于的时候把mid给start
先判断end，再判断start

二分法模板要点：
1. while(start + 1 < end) 避免死循环，这时退出的时候有start和end两个未作检查
2. 两个数的时候，有两个数返回
3. 避免int溢出
4. 可以不+1 -1

## 链表题目
1. 画图！
1. 写头节点的前一个结点
2. 三个节点指针，prev，curr，temp
2. while(里面都是放的不带next的)，只有判断环时候用的next

## 二叉树
**善用迭代法**\
前序遍历：需要栈的辅助\
根左右，root作为curr开始，检查条件为（栈非空 或 curr非空）\
如果curr非空，处理val，curr压栈，curr指向左\
如果curr为空，则其指向栈顶的元素的右节点，pop()退栈
```cpp
TreeNode* curr = root;
while (!stk.empty() || curr) {
    if (curr) {
        ans.push_back(curr->val);
        stk.push(curr);
        curr = curr->left;
    } else {
        curr = stk.top();
        stk.pop();
        curr = curr->right;
    }
}
```
中序遍历：需要栈的辅助，它和前序遍历唯一的区别就是，处理val的时机放到了curr为空的时候
```cpp
TreeNode* curr = root;
while (!stk.empty() || curr) {
    if (curr) {
        stk.push(curr);
        curr = curr->left;
    } else {
        curr = stk.top();
        stk.pop();
        ans.push_back(curr->val);
        curr = curr->right;
    }
}
```
后序遍历：左右根\
思路:两个栈，一个用于根右左遍历，一个用于保存序列\
实际上它做的是反向的先序遍历（根左右）。亦即遍历的顺序是：节点 -> 右子树 -> 左子树。\
这生成的是后根遍历的逆序输出。使用第二个栈，再执行一次反向输出即可得到所要的结果。\
```cpp
// 先序遍历，根右左
// normail
while (curr != nullptr || !stk.empty()) {
    if (curr) {
        ans.push_back(curr->val);
        stk.push(curr);
        curr = curr->left;
    } else {
        curr = stk.top();
        stk.pop();
        curr = curr->right;
    }
}
// 翻过来
while (curr || !stk.empty()) {
    if (curr) {
        ans.push_back(curr->val);
        // push another stack here
        stk.push(curr);
        curr = curr->right;
    } else {
        curr = stk.top();
        stk.pop();
        curr = curr->left;
    }
}
while (!stk2.empty()) {
    ans.push_back(stk2.top()->val);
    stk2.pop();
}
```
DFS:
1. 用迭代法
2. 入参有level参数，控制二维数组ans的行数
```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> ans;
    dfs(root, 1, ans);
    return ans;
}
void dfs(TreeNode* root, int level, vector<vector<int>>& ans) {
    if (root == nullptr) return;
    if (ans.size() < level) ans.push_back(vector<int>());
    ans[level-1].push_back(root->val);
    dfs(root->left, level+1, ans);
    dfs(root->right, level+1, ans);
}
```
BFS:
1. 用一个先入先出的队列，`queue`
2. 把root节点push进去，然后while循环只要queue不为空
```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> ans;
    if (!root) return ans;
    queue<TreeNode*> que;
    que.push(root);
    while (!que.empty()) {
        int n = que.size();
        ans.push(vector<int>());
        while (n > 0) {
            n--;
            TreeNode* temp = que.front();
            que.pop();
            ans.rbegin()->push_back(temp->val);
            if (temp->left) que.push(temp->left);
            if (temp->right) que.push(temp->right);
        }
    }
    return ans;
}
```

从遍历构造二叉树，必须有中序遍历。\
中序遍历需要构造hash表，方便快速定位左右子树的范围。

## 动态规划
1. 确定状态
    1. 最后一步
    2. 子问题
2. 状态方程
3. 边界和初始值
4. 迭代顺序

## 背包问题
### 01
例题中已知条件有第 i 个物品的重量 w_{i}，价值 v_{i}，以及背包的总容量 W。
设 DP 状态 f_{i,j} 为在只能放前 i 个物品的情况下，容量为 j 的背包所能达到的最大总价值。
```
dp[i][j] = max(dp[i-1][j], dp[i-1][j-w[i]] + v[i])
dp[j] = max(dp[j], dp[j-w[i]] + v[i])
```
外层用物品循环，内层用单个重量循环
### 完全背包
```
dp[i][j] = max(dp[i-1][j], dp[i][j-w[i]] + v[i])
```
### 能不能凑成target
dp[j]表示容量为j的时候，是否能凑成
```cpp
dp[j] = dp[j] || dp[j - w[i]]
```
### 到底有多少种组合
dp[j]表示容量为j的时候，一共能凑成多少种组合
```
dp[i][j] = dp[i-1][j] + dp[i-1][j-w[i]] （如果j-w[i]存在的前提下）
dp[j] = dp[j] + dp[j-w[i]]
```

## 堆排序
堆排序就是个完全二叉树\
父节点比子节点大\
堆排序不具备稳定性，相同值在左边的有可能跑到右边
```cpp
// 注意A[0]是空的, len并不是A[]的长度
void HeapAdjust(vector<int>& nums, int index, int len) {
    // 传进来的每个根节点，调整叶子节点的顺序
    int temp = nums[index];
    for (int i=2*index; i<=len; i*=2) {
        if (i<len-1 && nums[i] < nums[i+1])
            i++;
        if (temp >= nums[i]) break;
        nums[index] = nums[i];
        index = i;
    }
    nums[index] = temp;
}
void buildheap(vector<int>& nums) {
    // 只用管非叶子节点，数量一共是len/2个
    // 方向从右向左，从底向上
    int len = nums.size()-1;
    for (int i=len/2; i>0; i--)
        HeapAdjust(nums, i, len);    
}
// 堆排序
void heapsort(vector<int>& nums, int len) {
    buildheap(nums);
    for (int i=len; i>0; i--) {
        swap(nums[1], nums[i]);
        HeapAdjust(nums, 1, i-1);    
    }
}
int main(){
    vector<int> vec{49,48,97,76,13,27,49};
    vec.insert(vec.begin(), 0);
    heapsort(vec, 7);
}
```

## 快速排序

模板:
```cpp
void quicksort(vector<int> & nums, int l, int r) {
    if (l >= r) return;
    int pivot = nums[l];
    int i = l, j = r;
    while (i < j) {
        while (i < j && nums[j] >= pivot) j--;
        nums[i] = nums[j];
        while (i < j && nums[i] <= pivot) i++;
        nums[j] = nums[i];
    }
    nums[i] = pivot;
    quicksort(nums, l, i - 1);
    quicksort(nums, i + 1, r);
}
```