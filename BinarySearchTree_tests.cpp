// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>


TEST(test_empty_size_height_copy_destroy) {
    // Add your tests here
    BinarySearchTree<int> tree;
    
    //EMPTY
    ASSERT_TRUE(tree.empty());

    //SIZE
    ASSERT_EQUAL(0, tree.size());

    //HEIGHT
    ASSERT_EQUAL(0, tree.height());

    tree.insert(1);
    tree.insert(4);
    tree.insert(3);
    tree.insert(5);

    //SIZE
    ASSERT_EQUAL(4, tree.size());

    //HEIGHT
    ASSERT_EQUAL(3, tree.height());

    //COPY CONSTRUCTOR
    BinarySearchTree<int> tree2;
    tree2 = tree;
    ASSERT_EQUAL(tree.size(), tree2.size());
    ASSERT_EQUAL(tree.height(), tree2.height());
    
    ASSERT_EQUAL(1, *tree.find(1));
    ASSERT_EQUAL(3, *tree.find(3));
    ASSERT_EQUAL(5, *tree.find(5));

    ASSERT_EQUAL(1, *(tree.min_element()));
    ASSERT_EQUAL(5, *(tree.max_element()));

    //Check sorting invariant
    ASSERT_TRUE(tree.check_sorting_invariant());

    std::ostringstream oss_preorder2;
    tree.traverse_preorder(oss_preorder2);
    std::cout << "preorder" << std::endl;
    std::cout << oss_preorder2.str() << std::endl << std::endl;
    ASSERT_EQUAL(oss_preorder2.str(), "1 4 3 5 ");

    std::ostringstream oss_preorder4;
    tree.traverse_inorder(oss_preorder4);
    std::cout << "inorder" << std::endl;
    std::cout << oss_preorder4.str() << std::endl << std::endl;
    ASSERT_EQUAL(oss_preorder4.str(), "1 3 4 5 ");
    
    //change first datum to 2, resulting in the first broken tree above
    *tree.begin() = 10;
    ASSERT_FALSE(tree.check_sorting_invariant());

    std::ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    std::cout << "preorder" << std::endl;
    std::cout << oss_preorder.str() << std::endl << std::endl;
    ASSERT_EQUAL(oss_preorder.str(), "10 4 3 5 ");

    std::ostringstream oss_preorder3;
    tree.traverse_inorder(oss_preorder3);
    std::cout << "inorder" << std::endl;
    std::cout << oss_preorder3.str() << std::endl << std::endl;
    ASSERT_EQUAL(oss_preorder3.str(), "10 3 4 5 ");
}


TEST(test_empty_size_height_copy_destroy_2) {
    // Add your tests here
    BinarySearchTree<int> tree;

    tree.insert(5);
    tree.insert(4);
    tree.insert(1);
    tree.insert(3);
    

    //SIZE
    ASSERT_EQUAL(4, tree.size());

    //HEIGHT
    ASSERT_EQUAL(4, tree.height());

    //COPY CONSTRUCTOR
    BinarySearchTree<int> tree2;
    tree2 = tree;
    ASSERT_EQUAL(tree.size(), tree2.size());
    ASSERT_EQUAL(tree.height(), tree2.height());
    
    ASSERT_EQUAL(1, *tree2.find(1));
    ASSERT_EQUAL(3, *tree2.find(3));
    ASSERT_EQUAL(5, *tree2.find(5));

    ASSERT_EQUAL(1, *(tree2.min_element()));
    ASSERT_EQUAL(5, *(tree2.max_element()));

    //CHECK SORTING INVARIANT
    ASSERT_TRUE(tree2.check_sorting_invariant());

    //change first datum to 2, resulting in the first broken tree above
    *tree2.begin() = 2;
    ASSERT_TRUE(tree2.check_sorting_invariant());

    //MIN GREATER THAN
    ASSERT_EQUAL(*tree.min_greater_than(2), 3);
    ASSERT_EQUAL(*tree.min_greater_than(4), 5);
    ASSERT_TRUE(tree.min_greater_than(5) == tree.end());
    ASSERT_EQUAL(*tree.min_greater_than(-1), 1);
}

TEST(sorting_test) {
    BinarySearchTree<int> b;
    b.insert(1);
    b.insert(0);
    //change first datum to 2, resulting in the first broken tree above
    *b.begin() = 2;
    ASSERT_FALSE(b.check_sorting_invariant());

}

TEST(sorting_test2) {
    BinarySearchTree<int> b;
    b.insert(1);
    b.insert(0);
    b.insert(5);
    b.insert(200);
    b.insert(10);
    
    //change first datum to 2, resulting in the first broken tree above
    BinarySearchTree<int>::Iterator it;
    it = b.begin();
    ++it;
    *it = -12;
    ASSERT_FALSE(b.check_sorting_invariant());
    
    
    std::ostringstream oss_preorder;
    b.traverse_preorder(oss_preorder);
    std::cout << "preorder" << std::endl;
    std::cout << oss_preorder.str() << std::endl << std::endl;
    ASSERT_EQUAL(oss_preorder.str(), "-12 0 5 200 10 ");

    std::ostringstream oss_preorder3;
    b.traverse_inorder(oss_preorder3);
    std::cout << "inorder" << std::endl;
    std::cout << oss_preorder3.str() << std::endl << std::endl;
    ASSERT_EQUAL(oss_preorder3.str(), "0 -12 5 10 200 ");
}



TEST(test_empty_size_height_copy_destroy_3) {
    // Add your tests here
    BinarySearchTree<int> tree;

    tree.insert(5);
    tree.insert(4);
    tree.insert(1);
    tree.insert(7);
    tree.insert(10);
    tree.insert(2);
    tree.insert(15);

    

    //SIZE
    ASSERT_EQUAL(7, tree.size());

    //HEIGHT
    ASSERT_EQUAL(4, tree.height());

    //COPY CONSTRUCTOR
    BinarySearchTree<int> tree2;
    tree2 = tree;
    ASSERT_EQUAL(tree.size(), tree2.size());
    ASSERT_EQUAL(tree.height(), tree2.height());
    
    ASSERT_EQUAL(1, *tree2.find(1));
    ASSERT_EQUAL(tree2.end(), tree2.find(3));
    ASSERT_EQUAL(5, *tree2.find(5));
    ASSERT_EQUAL(tree2.end(), tree2.find(13));


    ASSERT_EQUAL(1, *(tree2.min_element()));
    ASSERT_EQUAL(15, *(tree2.max_element()));

    //CHECK SORTING INVARIANT
    ASSERT_TRUE(tree2.check_sorting_invariant());

    *tree2.begin() = 2;
    ASSERT_FALSE(tree2.check_sorting_invariant());

    //MIN GREATER THAN
    ASSERT_EQUAL(*tree.min_greater_than(2), 4);
    ASSERT_EQUAL(*tree.min_greater_than(4), 5);
    ASSERT_EQUAL(*tree.min_greater_than(5), 7);
    ASSERT_EQUAL(*tree.min_greater_than(-1), 1);
    
    
    *tree2.begin() = 12;
    ASSERT_FALSE(tree2.check_sorting_invariant())
}


TEST(empty_tree) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());

    //SIZE
    ASSERT_EQUAL(0, tree.size());

    //HEIGHT
    ASSERT_EQUAL(0, tree.height());
    
    ASSERT_EQUAL(tree.find(4), tree.end());
    
    ASSERT_EQUAL(tree.min_element(), tree.end());
    ASSERT_EQUAL(tree.max_element(), tree.end());
    ASSERT_TRUE(tree.empty());
    ASSERT_EQUAL(tree.min_greater_than(5), tree.end());
}


TEST_MAIN()
