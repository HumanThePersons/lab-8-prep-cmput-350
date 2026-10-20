// maybe something here...
#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    // Constructor : Initializes scale to (1,1) and translation to (0,0) if none given (identity transformations)
    BasicTransform(sf::Vector2f scale = {1,1}, sf::Vector2f translation = {0,0}) 
        : scale(scale), translation(translation) {}

    // Apply transformations on given point, does not modify original point (const) instead returns a new point
    sf::Vector2f applyPoint(const sf::Vector2f& p) const {
        // This is equivalent to multiplying by the translation matrices and then omitting the 1
        sf::Vector2f transformedPoint = p*scale;  // Equivalent to (p.x * scale.x, p.y * scale.y) using vector_util *
        return (transformedPoint += translation);
    }

    // Apply transformations on given vector, does not modify the original vector (const) instead returns a new vector
    sf::Vector2f applyVector(const sf::Vector2f& p) const {
        // Translation will not impact a vector, since the two endpoints will always have the same relative 
        // distance (the vector is the difference between these two points)
        return p*scale;  // Only need to apply scale
    }

    // Checks if 'other' has equal scale and translation transformations to this
    bool operator==(const BasicTransform& other) {
        return ((other.scale == scale) && (other.translation == translation));
    }

    sf::Vector2f scale;
    sf::Vector2f translation;
};
