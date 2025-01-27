#include <catch2/catch.hpp>
#include "../../source/amr/Mesh.inl"
#include <iostream>

using namespace AMR;


struct GridI64 : GridConfig<i64>
{
    static void upsample(Array<i64, 2>::Getter src, Array<i64, 2>::Getter dst) {
        dst(0l, 0l) = dst(0l, 1l) = dst(1l, 0l) = dst(1l, 1l) = src(0l, 0l);
    }
    static void downsample(Array<i64, 2>::Getter src, Array<i64, 2>::Getter dst) {
        dst(0l, 0l) = (src(0l, 0l) + src(1l, 0l) + src(0l, 1l) + src(1l, 1l)) / 4;
    }
};

using Config2D = MeshConfig<2, 8, GridI64>;


TEST_CASE("Create Node", "[mesh]")
{
    auto buffers = Config2D::createBuffers(16);
    Node<Config2D> node(nullptr, buffers, 0, 0);
    auto buffers2 = Config2D::createBuffers(16);
    node.split(buffers2, 0);
}

/*TEST_CASE("Create Tree", "[mesh]")
{
    Tree<Config2D> tree;
}

TEST_CASE("Restructure Tree", "[mesh]")
{
    Tree<Config2D> tree;
    tree.restructure();

    tree.root->action = AMR::Refine;
    tree.restructure();
    CHECK(!tree.root->isLeaf);
    CHECK(tree.root->children[{0, 0}]->isLeaf);
    CHECK(tree.root->children[{0, 1}]->isLeaf);
    CHECK(tree.root->children[{1, 0}]->isLeaf);
    CHECK(tree.root->children[{1, 1}]->isLeaf);

    tree.root->children[{0, 0}]->action = AMR::Coarsen;
    tree.restructure();
    CHECK(!tree.root->isLeaf);
    CHECK(tree.root->children[{0, 0}]->isLeaf);
    CHECK(tree.root->children[{0, 1}]->isLeaf);
    CHECK(tree.root->children[{1, 0}]->isLeaf);
    CHECK(tree.root->children[{1, 1}]->isLeaf);

    tree.root->children[{0, 0}]->action = AMR::Coarsen;
    tree.root->children[{0, 1}]->action = AMR::Coarsen;
    tree.root->children[{1, 0}]->action = AMR::Coarsen;
    tree.root->children[{1, 1}]->action = AMR::Coarsen;
    tree.restructure();
    CHECK(tree.root->isLeaf);
}

TEST_CASE("Synchronize Tree", "[mesh]")
{
    Tree<Config2D> tree;
    tree.root->action = AMR::Refine;
    tree.restructure();
    tree.synchronize();
}

TEST_CASE("Apply kernel", "[mesh]")
{
    Tree<Config2D> tree;
    tree.root->action = AMR::Refine;
    tree.restructure();
    tree.synchronize();
    tree.applyKernel([](DataView<Config2D> view) {
        view.get<0>(0, 0) = 4;
    });
    Loop<2>(0, 2, [&](auto& it1) {
        Loop<2>(1, Config2D::blockSize, [&](auto& it2) {
            CHECK(std::get<0>(tree.root->children[it1]->data)[it2] == 4);
        });
    });
}

TEST_CASE("Construct Mesh")
{
    Mesh<Config2D> mesh({5, 5});
    mesh.applyKernel([](DataView<Config2D> view) {
        view.get<0>(0, 0) = 4;
    });
}*/
