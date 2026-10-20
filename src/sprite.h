#ifndef SPRITE_H
#define SPRITE_H

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr, but not
                // necessary.
public:
    Sprite(const sf::Texture& texture) {
        // Constructor : No custom texMinPoint and texManPoint, so use texture size
        mMinPointTex = sf::Vector2f(0,0);
        mMaxPointTex = static_cast<sf::Vector2f>(texture.getSize());
        mTexture = &texture;
        mOrigin = sf::Vector2f(0,0);
    }

    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint) : mMinPointTex(texMinPoint), mMaxPointTex(texMaxPoint) {
        // Constructor : Custom min and max tex points given
        mTexture = &texture;
        mOrigin = sf::Vector2f(0,0);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        recomputeVertexArray();
        states.texture = mTexture; // States needs to be set to use the texture
        target.draw(mVertexArray, states);
    }

    sf::Vector2f& position() { return mTransform.translation; }

    const sf::Vector2f& position() const { return mTransform.translation; }

    sf::Vector2f& scale() { return mTransform.scale; }

    const sf::Vector2f& scale() const { return mTransform.scale; }

    sf::Vector2f& origin() { return mOrigin; }

    const sf::Vector2f& origin() const { return mOrigin; }

private:
    // It may help you to write this, which is an array of 6 vertices:
    //   - the first 3 correspond to the vertices of the top-left triangle of a "default rectangle"
    //     ranging from [0, 1] in both dimensions.
    //   - the next 3 correspond to the vertices of the bottom-right triangle. of the default
    //     rectangle.
    static constexpr std::array<sf::Vector2f, 6> DEFAULT_VERTICES = {
        sf::Vector2f(0,0), sf::Vector2f(0,1), sf::Vector2f(1,0),
        sf::Vector2f(1,1), sf::Vector2f(1,0), sf::Vector2f(0,1)
    };

    // if mTexture is valid, recomputes the positions and coords of sf::Vertex for each
    // of 6 possible entries in mVertexArray.
    void recomputeVertexArray() const {
        if (!mTexture) { return; }

        // Based on texture size, a point can be normalized by dividing by x and y respectively
        sf::Vector2f textureSize = mMaxPointTex - mMinPointTex;

        mVertexArray.clear();  // Remove the current vertices to make space for the new ones
        mVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        for (sf::Vector2f vertex : DEFAULT_VERTICES) {
            // Texture coordinates are based on min and max points
            sf::Vertex newVertex;
            newVertex.texCoords = mMinPointTex + (vertex * textureSize);

            // Position can be found as the vertex point, offet by mOrigin, transformed into world space by mTransform
            newVertex.position = mTransform.applyPoint((vertex * textureSize) - mOrigin);  // mOrigin is relative to mMinPointTex

            mVertexArray.append(newVertex);
        }

    }

    const sf::Texture* mTexture;
    mutable sf::VertexArray mVertexArray;  // geometry, uv coords -- mutable means it can be changed
                                           //  even in const methods.
    sf::Vector2f mMinPointTex;             // AABB for subtexture min point along both x, y
    sf::Vector2f mMaxPointTex;             // AABB for subtexture max point along both x, y
    sf::Vector2f mOrigin;  // position in tex coords of the sprite origin, relative to minPoint
    BasicTransform mTransform;
};

#endif
