# ----------------------------
# Makefile Options
# ----------------------------

NAME = GameLife
ICON = icon.png
DESCRIPTION = "Flappy Bird for the CE"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
