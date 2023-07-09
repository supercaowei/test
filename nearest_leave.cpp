/**
 * @brief 
 * 给一个二叉树，每个节点的值都不同，
 * 再给个的节点值，找到最临近给定节点的叶节点
 * 最多500个点，每个点val就是索引，例如
 * 	     	   1
 *          / \
 *         2   3
 *        / 
 *       4  
 *      /
 *     5
 *    / \
 *   6   8
    / 
   7
 * 给定2的话，最近的是3，给定4的话，最近的是6。
 */

 /**
 * 二叉树节点定义
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
**/

/*
 * 新构造一个树结构TreeNode1，对自己的父节点保存访问
 * TODO: 把重复代码抽出来作为公用的方法
*/
struct TreeNode1 {
    int val_;
    TreeNode1* parent_, left_, right_;
    int left_end_istance = -1, right_end_istance = -1;
    int left_end_val = 0, right_end_val = 0;
    /int nearest_left_val_ = 0;
    //int nearest_left_distance_ = -1;

    //用给定的TreeNode树生成一个TreeNode1树，构造的时候计算出每个节点的子树的最近叶节点距离。
    //比如节点a有子节点b和c，则b到c的距离不会比a到c的距离小，因此构造时只需要计算孩子节点的最近叶节点。
    TreeNode1(TreeNode* node) {
        if (!node) {
          return;
        }
        val_ = node->val;

        if (node->left) {
            left_ = new TreeNode1(node->left);
            left_->parent_ = this;
            if (left_->left_end_istance >= 0) {
                if (left_end_istance < 0 || left_end_istance > left_->left_end_istance + 1) {
                    left_end_istance = left_->left_end_istance + 1;
                    left_end_val = left_->left_end_val;
                }
            }
            if (left_->right_end_istance >= 0) {
                if (left_end_istance < 0 || left_end_istance > left_->right_end_istance + 1) {
                    left_end_istance = left_->right_end_istance + 1;
                    left_end_val = left_->right_end_val;
                }
            }
        }
        if (node->right) {
            right_ = new TreeNode1(node->right);
            right_->parent_ = this;
            if (right_->left_end_istance >= 0) {
                if (right_end_istance < 0 || right_end_istance > right_->left_end_istance + 1) {
                    right_end_istance = right_->left_end_istance + 1;
                    right_end_val = right_->left_end_val;
                }
            }
            if (right_->right_end_istance >= 0) {
                if (right_end_istance < 0 || right_end_istance > right_->right_end_istance + 1) {
                    right_end_istance = right_->right_end_istance + 1;
                    right_end_val = right_->right_end_val;
                }
            }
        }
    }
    
    //查找值为val的节点
    TreeNode1* find(int val) {
        if (val_ == val) {
           return this;
        }
        TreeNode1* leftFind = left_ ? left_->find(val) : NULL;
        if (leftFind) {
            return leftFind;
        }
        TreeNode1* rightFind = right_ ? right_->find(val) : NULL;
        if (rightFind) {
            return rightFind;
        }
        return NULL;
    }

    //计算TreeNode1节点的最近叶节点：比较左右孩子和parent的最近叶节点距离（左右孩子无需重新遍历）。
    int getNearestLeavesNode() {
        if (!left_ && !right_) {
           return val_;
        }
        int distance = -1;
        int nearest_val = 0;
        if (left_) {
          if (distance < 0 || distance > left_->left_end_istance + 1) {
            nearest_val = left_->left_end_val;
          }
          if (distance < 0 || distance > left_->right_end_istance + 1) {
            nearest_val = left_->right_end_val;
          }
        }
        if (right_) {
          if (distance < 0 || distance > right_->left_end_istance + 1) {
            nearest_val = right_->left_end_val;
          }
          if (distance < 0 || distance > right_->right_end_istance + 1) {
            nearest_val = right_->right_end_val;
          }
        }
        if (parent_) {
          if (parent_->left_ != this) {
            if (distance < 0 || distance > parent_->left_->getNearestLeavesNode() + 1) {
              nearest_val = parent_->left_->getNearestLeavesNode() + 1;
            }
            
          }
          if (parent_->right_ != this) {
            if (distance < 0 || distance > parent_->right->getNearestLeavesNode() + 1) {
              nearest_val = parent_->right_->getNearestLeavesNode() + 1;
            }
          }
          if (parent_->parent_) {
            if (distance < 0 || distance > parent_->parent_->getNearestLeavesNode() + 1) {
              nearest_val = parent_->parent_->getNearestLeavesNode() + 1;
            }
          }
        }
    }
};


