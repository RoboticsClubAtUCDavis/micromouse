#include "../Direction.h"
#include "catch.hpp"

SCENARIO("DirectionVectors behave correctly") {
    GIVEN("A default DirectionVector") {
        DirectionVector directionVector;

        REQUIRE(directionVector.direction == NONE);
        REQUIRE(directionVector.magnitude == 0);

        WHEN("The magnitude is changed")
        {
            directionVector.magnitude = 2;

            THEN("The change is reflected")
            {
                REQUIRE(directionVector.direction == NONE);
                REQUIRE(directionVector.magnitude == 2);
            }
        }

        WHEN("The direction is changed")
        {
            directionVector.direction = W;

            THEN("The change is reflected")
            {
                REQUIRE(directionVector.direction == W);
                REQUIRE(directionVector.magnitude == 0);
            }
        }

        WHEN("The object is assigned")
        {
            directionVector.direction = SW;
            directionVector.magnitude = 7;

            directionVector = DirectionVector();

            THEN("The assignment occurs")
            {
                REQUIRE(directionVector.direction == NONE);
                REQUIRE(directionVector.magnitude == 0);
            }
        }
    }

    GIVEN("A custom DirectionVector") {
        DirectionVector directionVector(E, 7);

        REQUIRE(directionVector.direction == E);
        REQUIRE(directionVector.magnitude == 7);
    }
}