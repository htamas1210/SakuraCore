#include <catch2/catch_test_macros.hpp>

#include "Layer.h"
#include "LayerStack.h"

#include <memory>
#include <vector>

TEST_CASE("Layer operations", "[Layer]") {
    SakuraVNE::LayerStack lstack;
    const auto &layers = lstack.GetLayers();

    // Create the unique_ptr, keep a raw pointer for testing Pop operations later, then move it.
    auto u_layer1 = std::make_unique<SakuraVNE::Layer>("layer1");
    SakuraVNE::Layer *layer1 = u_layer1.get();
    lstack.PushLayer(std::move(u_layer1));

    SECTION("PushLayer should make the size 2") {
        auto u_layer2 = std::make_unique<SakuraVNE::Layer>("layer2");
        lstack.PushLayer(std::move(u_layer2));
        REQUIRE(layers.size() == 2);
        REQUIRE(layers[1]->GetName() == "layer2");
    }
    SECTION("PopLayer should make the size 0") {
        lstack.PopLayer(layer1);
        REQUIRE(layers.size() == 0);
    }
    SECTION("PushOverlay should always put at the end of the list and should always be after the last fence index") {
        auto u_layer2 = std::make_unique<SakuraVNE::Layer>("layer2");
        lstack.PushOverLay(std::move(u_layer2));
        REQUIRE(layers[1]->GetName() == "layer2");

        auto u_layer3 = std::make_unique<SakuraVNE::Layer>("layer3");
        lstack.PushLayer(std::move(u_layer3));

        REQUIRE(layers[1]->GetName() == "layer3");
        REQUIRE(layers[2]->GetName() == "layer2");
    }
    SECTION("Pop overlay") {
        auto u_layer6 = std::make_unique<SakuraVNE::Layer>("layer6");
        SakuraVNE::Layer *layer6 = u_layer6.get();
        lstack.PushOverLay(std::move(u_layer6));
        REQUIRE(layers[1]->GetName() == "layer6");

        lstack.PopOverlay(layer6);

        bool hasLayer2InVec = false;
        for (auto &layer : layers) {
            if (layer->GetName() == "layer6") {
                hasLayer2InVec = true;
            }
        }

        REQUIRE(hasLayer2InVec == false);
    }
}
