#ifndef BULLET_HELL_SOUL_HPP
#define BULLET_HELL_SOUL_HPP

class Soul
{
    public:

        //Constructor
        // Initializes the Soul with a starting position and speed
        Soul(float startX, float startY, float speed);


        /*Movement methods
        ------------------------------------------------
        * deltatime: The time elapsed since the last frame
        * used to calculate the distance to move based 
        * on the speed of the Soul.
        ------------------------------------------------
        */
        
        void moveUp(float deltaTime);

        void moveDown(float deltaTime);

        void moveLeft(float deltaTime);

        void moveRight(float deltaTime);

        // Updates the short protection period after the Soul is hit.
        void updateInvincibility(float deltaTime);

        bool canTakeDamage() const;
        void startInvincibility(float duration);

        /* Garantees that the Soul will stay 
        within the boundaries of the room.*/

        void keepInsideRoom(float roomWidth, float roomHeight);

        /*
        The getters above are used to return both 
        the horizontal and vertical position of the Soul
        which is used to determine if the Soul has collided 
        with a bullet or if it has moved outside the boundaries of the room.
        */

        float getX() const;

        float getY() const;

        // Returns the side length used for boundary and collision checks.
        float getSize() const;

    private:

        float x; // The horizontal position of the Soul
        float y; // The vertical position of the Soul
        float speed; // The speed at which the Soul moves
        float size; // The size of the Soul, used for collision detection
        float invincibilityTime;
};

#endif
