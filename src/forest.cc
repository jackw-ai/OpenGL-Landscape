// #include "forest.h"
// #include <iostream>

// Forest::Forest() {
// }

// Forest::Forest(std::vector<Tree*> init_trees) {
//     trees = init_trees;
// }

// void Forest::setNumBranchRendered(int num_branch_rendered) {
//     for(int i = 0; i < trees.size(); i++) {
//         trees[i]->setNumBranchRendered(num_branch_rendered);
//     }
// }

// void Forest::setRenderLeaves(bool render_leaves) {
//     for(int i = 0; i < trees.size(); i++) {
//         trees[i]->setRenderLeaves(render_leaves);
//     }
// }

// void Forest::growUpward(bool* keys) {
//     for(int i = 0; i < trees.size(); i++) {
//         numBranchRendered += 2;

//         if (numBranchRendered > (int)branchList.size()) {
//             numBranchRendered = (int)branchList.size();
//         }
//         else if (numBranchRendered < 0) {
//             numBranchRendered = 0;
//         }

// }
// void Forest::growDownward(bool* keys) {

// }

// void Tree::keyControl(bool* keys) {   
// 	if (keys[GLFW_KEY_G]) {
// 		growUpward(keys);
// 	}

// 	if (keys[GLFW_KEY_B]) {
// 		growDownward(keys);
// 	}

// 	if (keys[GLFW_KEY_L]) {
// 		std::cout << "toggle leaves" << std::endl;
// 		render_leaves = !render_leaves;
// 	}
// }

// Forest::~Forest() {
// }