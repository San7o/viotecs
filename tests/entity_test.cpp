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

#include "valfuzz/valfuzz.hpp"
#include "viotecs/viotecs.hpp"

using namespace viotecs;

TEST(entity, "Create and remove and entity")
{
    world::init();

    entity_t entity = world::new_entity();
    ASSERT(entity != -1);

    auto entities = world::get_entities();
    ASSERT(entities != nullptr);
    ASSERT(entities->size() == 1);

    world::remove_entity(entity);
    entities = world::get_entities();
    ASSERT(entities != nullptr);
    ASSERT(entities->size() == 0);

    world::destroy();
}

TEST(entities, "Create a bunch of new entities")
{
    world::init();

    entity_t entity = world::new_entity();
    ASSERT(entity != -1);
    entity = world::new_entity();
    ASSERT(entity != -1);
    entity = world::new_entity();
    ASSERT(entity != -1);
    entity = world::new_entity();
    ASSERT(entity != -1);
    entity = world::new_entity();
    ASSERT(entity != -1);
    entity = world::new_entity();
    ASSERT(entity != -1);
    entity = world::new_entity();
    ASSERT(entity != -1);
    entity = world::new_entity();
    ASSERT(entity != -1);
    entity = world::new_entity();
    ASSERT(entity != -1);

    auto entities = world::get_entities();
    ASSERT(entities != nullptr);
    ASSERT(entities->size() == 9);

    world::destroy();
}
