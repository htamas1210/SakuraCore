#include <catch2/catch_test_macros.hpp>

#include "Layer.h"
#include "LayerStack.h"

#include <iostream>
#include <vector>

TEST_CASE("Layer operations", "[Layer]") {
    SakuraVNE::LayerStack lstack;
    const auto &layers = lstack.GetLayers();

    SakuraVNE::Layer *layer1 = new SakuraVNE::Layer("layer1");
    lstack.PushLayer(layer1);

    SECTION("PushLayer should make the size 2") {
        SakuraVNE::Layer *layer2 = new SakuraVNE::Layer("layer2");
        lstack.PushLayer(layer2);
        REQUIRE(layers.size() == 2);
        REQUIRE(layers[1]->GetName() == "layer2");
    }
    SECTION("PopLayer should make the size 0") {
        lstack.PopLayer(layer1);
        REQUIRE(layers.size() == 0);
    }
    SECTION("PushOverlay should always put at the end of the list and should always be after the last fence index") {
        SakuraVNE::Layer *layer2 = new SakuraVNE::Layer("layer2");
        lstack.PushOverLay(layer2);
        REQUIRE(layers[1]->GetName() == "layer2");

        SakuraVNE::Layer *layer3 = new SakuraVNE::Layer("layer3");
        lstack.PushLayer(layer3);

        REQUIRE(layers[1]->GetName() == "layer3");
        REQUIRE(layers[2]->GetName() == "layer2");
    }
    SECTION("Pop overlay") {
        SakuraVNE::Layer *layer6 = new SakuraVNE::Layer("layer6");
        lstack.PushOverLay(layer6);
        REQUIRE(layers[1]->GetName() == "layer6");

        lstack.PopOverlay(layer6);

        bool hasLayer2InVec = false;
        for (auto layer : layers) {
            if (layer->GetName() == "layer6") {
                hasLayer2InVec = true;
            }
        }

        REQUIRE(hasLayer2InVec == false);
    }
}
