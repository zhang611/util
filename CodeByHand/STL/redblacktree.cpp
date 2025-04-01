Node *insert_maintain(Node *root) {
    if (!has_red_child(root)) return root;
    if (root->left->color == 0 && root->right->color == 0) {
        if (!has_red_child(root->left) && !has_red_child(root->right))
            return root;
        root->color = 0;
        root->left->color = root->right->color = 1;
        return root;
    }
    if (root->left->color == 0 && !has_red_child(root->left)) return root;
    if (root->right->color == 0 && !has_red_child(root->right)) return root;
    if (root->left->color == 0) {
        if (root->left->right->color == 0) {
            root->left = left_rotate(root->left);
        }
        root = right_rotate(root);
        root->color = 0;
        root->left->color = root->right->color = 1;
    } else {
        if (root->right->left->color == 0) {
            root->right = right_rotate(root->right);
        }
        root = left_rotate(root);
        root->color = 0;
        root->left->color = root->right->color = 1;
    }
    return root;
}

Node *erase_maintain(Node *root) {
    if (root->left->color < 2 && root->right->color < 2) return root;
    if (has_red_child(root)) {
        root->color = 0;
        int flag = 0;
        if (root->left->color == 0) {
            root = right_rotate(root);
            flag = 0;
            // root->color = 1;
            // root->right = erase_maintain(root->right);
        } else {
            root = left_rotate(root);
            flag = 1;
            // root->color = 1;
            // root->left = erase_maintain(root->left);
        }
        root->color = 1;
        if (flag == 0)
            root->right = erase_maintain(root->right);
        else
            root->left = erase_maintain(root->left);
        return root;
    }
    
    if ((root->left->color == 2 && !has_red_child(root->right)) ||
        (root->right->color == 2 && !has_red_child(root->left))) {
        root->color += 1;
        root->left->color -= 1;
        root->right->color -= 1;
        return root;
    }
    if (root->left->color == 2) {
        root->left->color = 1;
        if (root->right->right->color != 0) {
            root->right->color = 0;
            root->right = right_rotate(root->right);
            root->right->color = 1;
        }
        root->right->color = root->color;
        root = left_rotate(root);
        // root->color = root->left->color;
        root->right->color = root->left->color = 1;
    } else {
        root->right->color = 1;
        if (root->left->left->color != 0) {
            root->left->color = 0;
            root->left = left_rotate(root->left);
            root->left->color = 1;
        }
        root->left->color = root->color;
        root = right_rotate(root);
        // root->color = root->right->color;
        root->left->color = root->right->color = 1;
    }
    return root;
}