#include "../Coordinate.h"
#include "../Direction.h"
#include "catch.hpp"

SCENARIO("NodeCoordinates have an x and y component") {
    GIVEN("x and y") {
        int x = 5;
        int y = 2;
        WHEN("A coordinate is created") {
            NodeCoordinate c = NodeCoordinate(x, y);
            THEN("x and y are defined") {
                REQUIRE(c.x != NULL);
                REQUIRE(c.y != NULL);
            }
            THEN("x and y are stored") {
                REQUIRE(c.x == x);
                REQUIRE(c.y == y);
            }
        }
    }
}
SCENARIO("CellCoordinates have an x and y component") {
    GIVEN("x and y") {
        int x = 5;
        int y = 2;
        WHEN("A coordinate is created") {
            CellCoordinate c = CellCoordinate(x, y);
            THEN("x and y are defined") {
                REQUIRE(c.x != NULL);
                REQUIRE(c.y != NULL);
            }
            THEN("x and y are stored") {
                REQUIRE(c.x == x);
                REQUIRE(c.y == y);
            }
        }
    }
}
SCENARIO("CellCoordinates can be converted into NodeCoordinates") {
    GIVEN("a CellCoordinate") {
        CellCoordinate a = CellCoordinate(5, 4);
        WHEN("it is converted using toNode()") {
            NodeCoordinate b = a.toNode();
            THEN("x = 2x + 1") {
                REQUIRE(b.x == 2 * a.x + 1);
            }
            THEN("y = 2y + 1") {
                REQUIRE(b.y == 2 * a.y + 1);
            }
        }
        WHEN("it is converted using cast") {
            NodeCoordinate b = (NodeCoordinate)a;
            THEN("x = 2x + 1") {
                REQUIRE(b.x == 2 * a.x + 1);
            }
            THEN("y = 2y + 1") {
                REQUIRE(b.y == 2 * a.y + 1);
            }
        }
    }
}
SCENARIO("Direction should be cast to coordinate") {
    GIVEN("a Direction") {
        WHEN("NW") {
            THEN("-1, 1") {
                REQUIRE(NodeCoordinate(NW).x == NodeCoordinate(-1, 1).x);
                REQUIRE(NodeCoordinate(NW).y == NodeCoordinate(-1, 1).y);
            }
        }
        WHEN("NE") {
            THEN("1, 1") {
                REQUIRE(NodeCoordinate(NE).x == NodeCoordinate(1, 1).x);
                REQUIRE(NodeCoordinate(NE).y == NodeCoordinate(1, 1).y);
            }
        }
        WHEN("N") {
            THEN("0, 1") {
                REQUIRE(NodeCoordinate(N).x == NodeCoordinate(0, 1).x);
                REQUIRE(NodeCoordinate(N).y == NodeCoordinate(0, 1).y);
            }
        }
        WHEN("E") {
            THEN("1, 0") {
                REQUIRE(NodeCoordinate(E).x == NodeCoordinate(1, 0).x);
                REQUIRE(NodeCoordinate(E).y == NodeCoordinate(1, 0).y);
            }
        }
        WHEN("W") {
            THEN("-1, 0") {
                REQUIRE(NodeCoordinate(W).x == NodeCoordinate(-1, 0).x);
                REQUIRE(NodeCoordinate(W).y == NodeCoordinate(-1, 0).y);
            }
        }
        WHEN("S") {
            THEN("0, -1") {
                REQUIRE(NodeCoordinate(S).x == NodeCoordinate(0, -1).x);
                REQUIRE(NodeCoordinate(S).y == NodeCoordinate(0, -1).y);
            }
        }
        WHEN("SE") {
            THEN("1, -1") {
                REQUIRE(NodeCoordinate(SE).x == NodeCoordinate(1, -1).x);
                REQUIRE(NodeCoordinate(SE).y == NodeCoordinate(1, -1).y);
            }
        }
        WHEN("SW") {
            THEN("-1, -1") {
                REQUIRE(NodeCoordinate(SW).x == NodeCoordinate(-1, -1).x);
                REQUIRE(NodeCoordinate(SW).y == NodeCoordinate(-1, -1).y);
            }
        }
        WHEN("NONE") {
            THEN("0, 0") {
                REQUIRE(NodeCoordinate(NONE).x == NodeCoordinate(0, 0).x);
                REQUIRE(NodeCoordinate(NONE).y == NodeCoordinate(0, 0).y);
            }
        }
    }
}
SCENARIO("Adding Coordinates together") {
    GIVEN("two CellCoordinates") {
        CellCoordinate a = CellCoordinate(1, 2);
        CellCoordinate b = CellCoordinate(2, 3);
        WHEN("+") {
            CellCoordinate c = a + b;
            THEN("x components add up") {
                REQUIRE(c.x == a.x + b.x);
            }
            THEN("y components add up") {
                REQUIRE(c.y == a.y + b.y);
            }
        }
        WHEN("-") {
            CellCoordinate c = a - b;
            THEN("x components subtract") {
                REQUIRE(c.x == a.x - b.x);
            }
            THEN("y components subtract") {
                REQUIRE(c.y == a.y - b.y);
            }
        }
    }
    GIVEN("two NodeCoordinates") {
        NodeCoordinate a = NodeCoordinate(1, 2);
        NodeCoordinate b = NodeCoordinate(2, 3);
        WHEN("+") {
            NodeCoordinate c = a + b;
            THEN("x components add up") {
                REQUIRE(c.x == a.x + b.x);
            }
            THEN("y components add up") {
                REQUIRE(c.y == a.y + b.y);
            }
        }
        WHEN("-") {
            NodeCoordinate c = a - b;
            THEN("x components subtract") {
                REQUIRE(c.x == a.x - b.x);
            }
            THEN("y components subtract") {
                REQUIRE(c.y == a.y - b.y);
            }
        }
    }
    GIVEN("a CellCoordinate and NodeCoordinate") {
        NodeCoordinate a = NodeCoordinate(1, 2);
        CellCoordinate b = CellCoordinate(2, 3);
        WHEN("c = a + b") {
            Coordinate c = a + b;
            THEN("The cellcoordinate is cast to a nodecoordinate") {
                REQUIRE(c.x == a.x + 2 * b.x + 1);
                REQUIRE(c.x == a.x + 2 * b.x + 1);
            }
        }
    }
    GIVEN("a NodeCoordinate and Direction") {
        NodeCoordinate a = NodeCoordinate(1, 2);
        WHEN("Direction is implicitly cast to Coordinate") {
            Direction b = NW;
            NodeCoordinate c = a + b;
            THEN("x = x + direction") {
                REQUIRE(c.x == a.x + NodeCoordinate(b).x);
            }
            THEN("y = y + direction") {
                REQUIRE(c.y == a.y + NodeCoordinate(b).y);
            }
        }
    }
}
SCENARIO("Equivalence of coordinates") {
    GIVEN("A NodeCoordinate") {
        NodeCoordinate a = NodeCoordinate(2, 5);
        WHEN("It is compared with itself") {
            THEN("It is equal") {
                REQUIRE(a == a);
                REQUIRE(!(a != a));
            }
        }
        WHEN("It is compared with something not equal") {
            NodeCoordinate b = NodeCoordinate(1, 5);
            THEN("Inequality is reflected") {
                REQUIRE(a != b);
                REQUIRE(!(a == b));
            }
        }
    }
}