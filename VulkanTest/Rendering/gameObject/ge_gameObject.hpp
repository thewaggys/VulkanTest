#pragma once

#include "../pipeline/model/GameEngine_model.hpp"
#include <memory>

namespace GameEngine {

    struct Transform2dComponent {
        glm::vec2 translation{};
        glm::vec2 scale{1.f,1.f};
        float rotation;

        glm::mat2 mat2() const {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c,s},{-s,c}};

            glm::mat2 scaleMatrix{{scale.x, .0f}, { .0f,scale.y}};
            return  rotMatrix * scaleMatrix;
        };
    };

    class GeGameObject {
        public:
        using id_t = unsigned int;

        static GeGameObject createGameObject() {
            static id_t currentId = 0;
            return GeGameObject(currentId++);
        };


        GeGameObject(const GeGameObject &) = delete;
        GeGameObject &operator=(const GeGameObject &) = delete;
        GeGameObject(GeGameObject&&) = default;
        GeGameObject &operator=(GeGameObject&&) = default;


        id_t getId() const { return id; }

        std::shared_ptr<LveModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

        explicit GeGameObject(id_t objId) : id(objId) {}
        id_t id;

    };

}