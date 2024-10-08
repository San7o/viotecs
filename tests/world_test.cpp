/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "oak/oak.hpp"
#include "valfuzz/valfuzz.hpp"
#include "viotecs/world.hpp"

using namespace viotecs;

BEFORE()
{
    oak::set_level(oak::level::disabled);
}

TEST(ecs_init, "Get ECS without inizialization")
{
    auto entities = world::get_entities();
    ASSERT(entities == nullptr);

    auto resources = world::get_resources();
    ASSERT(resources == nullptr);

    auto components = world::get_components();
    ASSERT(components == nullptr);
}

TEST(ecs_init_destroy, "ECS world inizialization and destruction")
{
    world::init();

    {
        auto entities = world::get_entities();
        ASSERT(entities != nullptr);

        auto resources = world::get_resources();
        ASSERT(resources != nullptr);

        auto components = world::get_components();
        ASSERT(components != nullptr);
    }

    world::destroy();

    {
        auto entities = world::get_entities();
        ASSERT(entities == nullptr);

        auto resources = world::get_resources();
        ASSERT(resources == nullptr);

        auto components = world::get_components();
        ASSERT(components == nullptr);
    }

    world::destroy();
}
