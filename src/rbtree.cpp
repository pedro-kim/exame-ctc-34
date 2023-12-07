#include "../includes/rbtree.h"

RBTree::RBTree() {
    _root = nullptr;
    return;
}

// RBTree::~RBTree() {
//     destroy_tree(root_);
//     return;
// }

void RBTree::make(const std::string& filename) {
    std::ifstream file(filename);
    std::string word;

    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo." << std::endl;
        return;
    }

    while (std::getline(file, word)) {
        add(word);
    }

    file.close();

    return;
}

void RBTree::add (const std::string& word ) { 
  Node* newNode = new Node(word);
  Node* current = _root;
  Node* aux = nullptr;

  if (_root == nullptr) {
    _root = newNode;
    _root->color = Node::BLACK;
  }
  // Encontrando o pai
  else {
    while (current != nullptr) {
      aux = current;
      if (newNode->word().compare(current->word())) current = current->leftchild;
      else current = current->rightchild;
    }
    newNode->parent = aux;
    // Inserindo o node como filho esquerdo ou direito
    if (newNode->word().compare(aux->word())) aux->leftchild = newNode;
    else aux->rightchild = newNode;

    // Balanceamento de arvore rubro negra
    while (newNode != _root && newNode->parent->color == Node::RED) {
      if (newNode->parent == newNode->parent->parent->leftchild) {
        current = newNode->parent->parent->rightchild;
        if (current && current->color == Node::RED) {
          //Caso 1: Tio vermelho
          newNode->parent->color = Node::BLACK;
          current->color = Node::BLACK;
          newNode->parent->parent->color = Node::RED;
          newNode = newNode->parent->parent;
        }
        else {
          if (newNode == newNode->parent->rightchild) {
            //Caso 2: tio preto e novo noh eh filho direito
            newNode = newNode->parent;
            rotateLeft(newNode);
          }
          //Caso 3: tio preto e novo noh eh filho esquerdo
          newNode->parent->color = Node::BLACK;
          newNode->parent->parent->color = Node::RED;
          rotateRight(newNode->parent->parent);
        }
      }
      else {
        current = newNode->parent->parent->leftchild;
        if (current && current->color == Node::RED) {
          newNode->parent->color = Node::BLACK;
          current->color = Node::BLACK;
          newNode->parent->parent->color = Node::RED;
          newNode = newNode->parent->parent;
        }
        else {
          if (newNode == newNode->parent->leftchild) {
            newNode = newNode->parent;
            rotateRight(newNode);
          }
          newNode->parent->color = Node::BLACK;
          newNode->parent->parent->color = Node::RED;
          rotateLeft(newNode->parent->parent);
        }
      }
    }
    _root->color = Node::BLACK;
  }
}

void RBTree::rotateRight (RBTree::Node* node) {
  Node* child = node->leftchild;
  node->leftchild = child->rightchild;
  if (child->rightchild != nullptr) child->rightchild->parent = node;
  child->parent = node->parent;
  if (node->parent == nullptr) _root = child;
  else if (node == node->parent->rightchild) node->parent->rightchild = child;
  else node->parent->leftchild = child;
  child->rightchild = node;
  node->parent = child;
}

void RBTree::rotateLeft (RBTree::Node* node) {
  Node* child = node->rightchild;
  node->rightchild = child->leftchild;
  if (child->leftchild != nullptr) child->leftchild->parent = node;
  child->parent = node->parent;
  if (node->parent == nullptr) _root = child;
  else if (node == node->parent->leftchild) node->parent->leftchild = child;
  else node->parent->rightchild = child;
  child->leftchild = node;
  node->parent = child;
}

void RBTree::find_prefix(const std::string& prefix) {
    std::vector<std::string> suggestions;
    Node* node = _root;

    find_prefix_recursive(node, prefix, suggestions);

    for(int i= 0; i < NUM_SUGGESTIONS && i < suggestions.size(); i++) {
        std::cout << suggestions[i] << std::endl;
    }

    return;

}

void RBTree::find_prefix_recursive(Node* node, const std::string& prefix, std::vector<std::string>& suggestions) {
        if (node == nullptr) {
            return;
        }
        if (node->word().compare(0, prefix.size(), prefix) == 0) {
            suggestions.push_back(node->word());
        }

        find_prefix_recursive(node->leftchild, prefix, suggestions);
        find_prefix_recursive(node->rightchild, prefix, suggestions);
}
