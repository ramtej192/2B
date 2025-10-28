#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAgood_pairs_SIZE 300005
#define MAgood_pairs_PRIME_VAL 4300000 
#define MAgood_pairs_BITS 23
#define INF 100000000

int phones = 0;

typedef struct TreeNode {
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* buildTree(int level_order[], int n) {
    if (n == 0 || level_order[0] == 0) return NULL;

    TreeNode** nodes = (TreeNode**)malloc(n * sizeof(TreeNode*));
    if (nodes == NULL) return NULL;
int i;
    for ( i = 0; i < n; i++) {
        if (level_order[i] == 1) {
            nodes[i] = (TreeNode*)malloc(sizeof(TreeNode));
            if (nodes[i] == NULL) return NULL;
            nodes[i]->left = nodes[i]->right = NULL;
        } else {
            nodes[i] = NULL;
        }
    }

    int parent = 0;
    int child = 1;
    while (child < n) {
        while (parent < n && nodes[parent] == NULL) {
            parent++;
        }
        if (parent >= n) break;

        if (child < n) {
            nodes[parent]->left = nodes[child];
            child++;
        }
        if (child < n) {
            nodes[parent]->right = nodes[child];
            child++;
        }
        parent++;
    }

    TreeNode* root = nodes[0];
    free(nodes);
    return root;
}

int fmp(TreeNode *root)
{//fmp = find min phones
 if(!root)return 2;
 int left=fmp(root->left),right=fmp(root->right);
 if(left==0||right==0){
   phones++;
   return 1;
 }
 if(right==1||left==1)return 2;
 return 0;
}

int min_phones(TreeNode*root)
{
   phones=0;
   int root_state = fmp(root);
   if(root_state == 0)
   {
       phones++;
   }
   return phones;
}

void freeTree(TreeNode* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

int primes[MAgood_pairs_SIZE];
int prime_count = 0;
char is_prime[MAgood_pairs_PRIME_VAL + 1];

void sieve(int m) {
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = 0;
int p;
    for ( p = 2; (long long)p * p <= MAgood_pairs_PRIME_VAL; p++) {
        if (is_prime[p]) {
        	int i;
            for ( i = p * p; i <= MAgood_pairs_PRIME_VAL; i += p)
                is_prime[i] = 0;
        }
    }

    prime_count = 0;
    for (p = 2; p <= MAgood_pairs_PRIME_VAL && prime_count < m; p++) {
        if (is_prime[p]) {
            primes[prime_count] = p;
            prime_count++;
        }
    }
}

typedef struct TrieNode {
    struct TrieNode* children[2];
    int count;
} TrieNode;

TrieNode* newTrieNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->children[0] = node->children[1] = NULL;
    node->count = 0;
    return node;
}

void insertTrie(TrieNode* node, int num) {
	int i;
    for ( i = MAgood_pairs_BITS - 1; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (node->children[bit] == NULL) {
            node->children[bit] = newTrieNode();
        }
        node = node->children[bit];
        node->count++;
    }
}

int queryTrie(TrieNode* node, int num, int K) {
    int count = 0;
    int i;
    for ( i = MAgood_pairs_BITS - 1; i >= 0; i--) {
        if (node == NULL) break;
        
        int num_bit = (num >> i) & 1;
        int k_bit = (K >> i) & 1;
        
        if (k_bit == 1) {
            if (node->children[num_bit] != NULL) {
                count += node->children[num_bit]->count;
            }
            node = node->children[1 - num_bit];
        } else {
            node = node->children[num_bit];
        }
    }
    return count;
}

void freeTrie(TrieNode* node) {
    if (node == NULL) return;
    freeTrie(node->children[0]);
    freeTrie(node->children[1]);
    free(node);
}

long long countPairsWithgood_pairsorLessOrEqual(int m, int K) {
    if (K < 0) return 0;
    TrieNode* root = newTrieNode();
    long long count = 0;
    int i;
    for ( i = 0; i < m; i++) {
        count += queryTrie(root, primes[i], K);
        insertTrie(root, primes[i]);
    }
    freeTrie(root);
    return count;
}

int main() {
    int n,i;
    scanf("%d", &n);

    int level_order[MAgood_pairs_SIZE];
    for (i = 0; i < n; i++) {
        scanf("%d", &level_order[i]);
    }

    TreeNode* root = buildTree(level_order, n);
    
    int m = min_phones(root);
    
    

    int l, r;
    scanf("%d %d", &l, &r);
    printf("%d\n", m);
    if (1) {
        sieve(m);
        
        long long right = countPairsWithgood_pairsorLessOrEqual(m, r);
        long long left = countPairsWithgood_pairsorLessOrEqual(m, l - 1);
        long long bad_pairs = right - left;
        
        long long total_pairs = (long long)m * (m - 1) / 2;
        long long good_pairs = total_pairs - bad_pairs;
        
        printf("%lld\n", good_pairs);
    }
    
    freeTree(root);
    return 0;
}

