#include <SFML/Graphics.hpp>

constexpr int windowX = { 600 }; // window width
constexpr int windowY = { 600 }; // window height
const sf::Color colour = sf::Color(255, 255, 0, 255); // custom colour. value between 0-255 for R,G,B,A

// make a class for rectangle shapes
class Rectangle
{
public:
	sf::RectangleShape shape;
	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getSize().x / 2.f; }
	float right() { return x() + shape.getSize().x / 2.f; }
	float top() { return y() - shape.getSize().y / 2.f; }
	float bottom() { return y() + shape.getSize().y / 2.f; } // Rectangle boundaries. The origin would be set to the center of the shape. 

};

// make a class of the Paddle
class Paddle : public Rectangle
{
public:
	const int velocity = 10; // paddle speed is constant, unless making powerups or debuff for it later
	sf::Vector2f speed;
	sf::Vector2f paddleSize = sf::Vector2f(200.f, 50.f); // size of the rectangle

	Paddle(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setSize(paddleSize);
		shape.setFillColor(colour);
		shape.setOrigin(paddleSize.x / 2.f, paddleSize.y / 2.f); // set origin to the center
	}

	// Check player input within Update function
	void Update()
	{
		shape.move(speed);
		// check if left is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && left() > 0)
		{
			speed.x = -velocity;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && right() < windowX)
		{
			speed.x = velocity;
		}
		else
		{
			speed.x = 0;
		}
	}
	
};

// make a class of the bricks
class Brick : public Rectangle
{
public:

private:

};

// make a class of ball
class Ball
{
public:
	int ballRadius = 10; // setup ball size
	int ballVelocity = 5;
	sf::CircleShape shape;
	sf::Vector2f velocity{ -ballVelocity, -ballVelocity };

	Ball(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setRadius(ballRadius);
		shape.setFillColor(sf::Color::Cyan);
		shape.setOrigin(ballRadius, ballRadius);
	}
public:
	float x() { return shape.getPosition().x; } // Reminder: Object origin is set to the center. 
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getRadius(); }
	float right() { return x() + shape.getRadius(); }
	float top() { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }

	void Update()
	{
		shape.move(velocity);
		// Set the window barrier for the ball
		if (left() < 0 )
		{
			velocity.x = ballVelocity;
		}
		else if (right() > windowX)
		{
			velocity.x = -ballVelocity;
		}

		if (top() < 0)
		{
			velocity.y = ballVelocity;
		}
		else if (bottom() > windowY)
		{
			velocity.y = -ballVelocity;
		}
	}
};

template <class c1, class c2>
bool isOverlapping(c1& mA, c2& mB)
{
	return mA.right() >= mB.left() && mA.left() <= mB.right() &&
		mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

void CollisionTest(Ball& mBall, Paddle& mPaddle)
{
	// check if the ball is overlapping with the paddle
	if (!isOverlapping(mBall, mPaddle))
	{
		return;
	}
	mBall.velocity.y = -mBall.ballVelocity;
	if (mBall.x() < mPaddle.x())
	{
		mBall.velocity.x = -mBall.ballVelocity;
	}
	else
	{
		mBall.velocity.x = mBall.ballVelocity;
	}

}

int main()
{
	// Render the window
	sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Breakout Hit!");
	window.setFramerateLimit(60); // stop the game from going insanely fast

	Paddle paddle{ windowX / 2, windowY / 1.5 }; // set paddle position
	Ball ball{ windowX / 2, windowY / 2 }; // set ball position

	// code runs while window is open
	while (window.isOpen())
	{
		paddle.Update();
		ball.Update();
		CollisionTest(ball, paddle);

		sf::Event event;
		//PollEvent is our window updating
		while (window.pollEvent(event))
		{
			// if window closed
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		//window.draw(paddle);
		//window.draw(ball);
		window.draw(paddle.shape);
		window.draw(ball.shape);
		window.display();
	}

	return 0;
}
