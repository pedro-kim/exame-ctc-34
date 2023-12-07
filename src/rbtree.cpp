#include "../includes/rbtree.h"

RBTree::RBTree() {
    root_ = nullptr;
    return;
}

RBTree::~RBTree() {
    destroy_tree(root_);
    return;
}

void RBTree::make(const std::string& filename) {
    std::ifstream file(filename);
    std::string word;

    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo." << std::endl;
        return;
    }

    int i = 0;
    while (std::getline(file, word) && i < 9) {
        insert_node(word);
        i++;
    }

    file.close();

    return;
}

void RBTree::insert_node(const std::string& word) {
    
    rbNode* node = new rbNode;
    rbNode* y = nullptr;
    rbNode* x = root_;

    node->word = word;
    node->left = nullptr;
    node->right = nullptr;
    node->color = true;
    node->parent = nullptr;

    while (x != nullptr) {
        y = x;
        if (node->word <= x->word) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    node->parent = y;

    if (y == nullptr) {
        root_ = node;
    } else if (node->word <= y->word) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == nullptr) {
        node->color = false;
        return;
    }

    if (node->parent->parent == nullptr) {
        return;
    }

    // node->left = nullptr;
    // node->right = nullptr;
    // node->color = true;

    insert_fixup(node);
    return;

}

void RBTree::insert_fixup(rbNode* node) {

    rbNode* uncle = nullptr;

    while (node->parent->color == true) {
        
        if (node->parent == node->parent->parent->left) {
            uncle = node->parent->parent->right;
            
            if (uncle->color == true) {
                uncle->color = false;
                node->parent->color = false;
                node->parent->parent->color = true;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rotate_left(node);
                }
                node->parent->color = false;
                node->parent->parent->color = true;
                rotate_right(node->parent->parent);
            }
        } else {
            uncle = node->parent->parent->left;
            if (uncle != nullptr){
                if (uncle->color == true) {
                    uncle->color = false;
                    node->parent->color = false;
                    node->parent->parent->color = true;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotate_right(node);
                    }
                    node->parent->color = false;
                    node->parent->parent->color = true;
                    rotate_left(node->parent->parent);
                }
            }
            else {
                if (node == node->parent->left) {
                        node = node->parent;
                        rotate_right(node);
                    }
                    node->parent->color = false;
                    node->parent->parent->color = true;
                    rotate_left(node->parent->parent);
            }
        }
        if (node == root_) {
            break;
        }
    }
    root_->color = false;
    return;
}

void RBTree::rotate_left(rbNode* node){
    rbNode* y = node->right;
    node->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = node;
    }

    y->parent = node->parent;
    if (node->parent == nullptr) {
        root_ = y;
    } else if (node == node->parent->left) {
        node->parent->left = y;
    } else {
        node->parent->right = y;
    }
    y->left = node;
    node->parent = y;
    return;
}

void RBTree::rotate_right(rbNode* node){
    rbNode* y = node->left;
    node->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = node;
    }

    y->parent = node->parent;
    if (node->parent == nullptr) {
        root_ = y;
    } else if (node == node->parent->right) {
        node->parent->right = y;
    } else {
        node->parent->left = y;
    }
    y->right = node;
    node->parent = y;
    return;
}

void RBTree::find_prefix(const std::string& prefix) {
    std::vector<std::string> suggestions;
    rbNode* node = root_;


    for(int i = 0; node != nullptr && i < NUM_SUGGESTIONS; i++) {
        if (node->word.compare(0, prefix.size(), prefix) == 0) {
            suggestions.push_back(node->word);
        } 
        
        if (node->word <= prefix) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    for(int i= 0; i < NUM_SUGGESTIONS && i < suggestions.size(); i++) {
        std::cout << suggestions[i] << std::endl;
    }

    return;

}

void RBTree::destroy_tree(rbNode* node) {
    if (node != nullptr) {
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }
    return;
}