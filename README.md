# NEAT

This is a basic example of NEAT algorithm.

Can be used as template for any project that requires NEAT. For that, check empty_template.cpp which is the minimal structure needed to properly run your program.

An example of implementation of NEAT can be found in snake_game.cpp, which is a snake game bot supervised by NEAT.

Based on the work of:
- Kenneth Stanley (https://www.cs.ucf.edu/~kstanley/neat.html)
- Neat AI (https://www.youtube.com/watch?v=3nbvrrdymF0&list=PLnICFpQDyZRFqjdtcTjshOb1IJqns6h6w)
- K.O. Stanley and R. Miikkulainen (http://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf)
- Kenneth O. Stanley, Bobby D. Bryant, Risto Miikkulainen (http://nn.cs.utexas.edu/downloads/papers/stanley.ieeetec05.pdf)

To run the snake AI:  <code>g++ snake_game.cpp -o snake_game.o -lsfml-window -lsfml-system -lsfml-graphics</code>
