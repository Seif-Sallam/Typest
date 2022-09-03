#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <random>
#include <time.h>

#include <fmt/core.h>

#include <SFML/Graphics.hpp>
std::mt19937 generator;

class Text : public sf::Drawable, public sf::Transformable
{
public:
	void enterColor(size_t index)
	{
		if (index >= textObjects.size())
			return;
		textObjects[index].setFillColor(enteredColor);
	}

	void setString(const sf::Font& font, const std::string& str)
	{
		this->str = str;
		textObjects.clear();
		for (uint32_t i = 0; i < str.size(); i++)
		{
			sf::Text txt;
			txt.setFont(font);
			txt.setString(sf::String(str[i]));
			txt.setFillColor(normalColor);
			txt.setCharacterSize(20);
			textObjects.push_back(txt);
		}
	}

	float getWidth() const
	{
		float width = 0.0f;

		for (auto& txt : textObjects)
			width += txt.getLocalBounds().width;

		return width;
	}


	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
	{
		states.transform *= this->getTransform();
		for (size_t i = 0; i < textObjects.size(); i++)
		{
			sf::Vector2f movement{i * +10.0f, 0.0f};
			sf::RenderStates copy = states;
			copy.transform.translate(movement);

			auto& txt = textObjects[i];
			target.draw(txt, copy);
		}
	}

	sf::Color enteredColor, normalColor;

private:
	std::string str;
	std::vector<sf::Text> textObjects;
};


void GenerateText(std::string& string, const std::vector<std::string>& words)
{
	static std::uniform_int_distribution<size_t> dist(0, words.size() - 1);
	string = words.at(dist(generator));
}

int main()
{
	srand(time(0));
	generator.seed(rand() % INT_MAX);
	sf::RenderWindow window(sf::VideoMode(800, 600), "Typest", sf::Style::None);
	sf::Font font;
	// if (!font.loadFromFile("rsc/Consolos.ttf"))
	if (!font.loadFromFile(RESOURCES_DIR"/Consolos.ttf"))
	{
		fmt::print("Coudln't find the font: rsc/Consolos.ttf.\n\tP.S. you can put any font there and name it Consolos.ttf nad it will work\n");
		system("pause");
		return 1;
	}

	sf::RectangleShape whiteSpace;
	whiteSpace.setSize(sf::Vector2f(window.getSize().x, 70.0f));
	whiteSpace.setPosition(0.0f, window.getSize().y / 2.0f - whiteSpace.getSize().y / 2.0f);

	std::string currentTextStr = "";

	std::vector<Text> texts;
	size_t currentTextIndex{};

	for (int i = 0; i < 10; i++)
	{
		Text txt;
		txt.setString(font, "");
		txt.normalColor = sf::Color::Black;
		txt.enteredColor = sf::Color(128, 128, 128);
		texts.push_back(txt);
	}
	std::vector<std::string> generatedWords;
	generatedWords.resize(texts.size());
	std::vector<std::string> availableWords;

	{
		// std::ifstream inputFile("rsc/Words.txt");
		std::ifstream inputFile(RESOURCES_DIR"/Words.txt");
		if(inputFile.fail())
		{
			fmt::print("could not open the words file in rsc/Words.txt.\n\tP.S. you can basically put a file with words in it and it will work, just make sure to put a word in each line\n");
			system("pause");
			return 1;
		}

		while(!inputFile.eof())
		{
			std::string line;
			inputFile >> line;
			availableWords.emplace_back(line);
		}
		inputFile.close();
	}
	// Setting the N strings
	for (size_t i = 0; i < texts.size(); i++)
	{
		GenerateText(generatedWords[i], availableWords);
		texts[i].setString(font, generatedWords[i]);
	}
	currentTextStr = generatedWords[0];

	char targetChar = currentTextStr[0];

	size_t index = 0;
	size_t maxSize = currentTextStr.size();
	size_t errors = 0;

	sf::Text typedText;
	typedText.setFont(font);
	typedText.setString("");
	typedText.setCharacterSize(20);
	typedText.setPosition(sf::Vector2f(window.getSize().x / 3.0f, 50.0f));

	sf::Text errorTxt;
	errorTxt.setFont(font);
	errorTxt.setPosition(50.0f, 40.0f);
	errorTxt.setCharacterSize(18);
	errorTxt.setString("Errors: 0");
	errorTxt.setFillColor(sf::Color::Red);
	sf::Text timeTxt;
	timeTxt.setFont(font);
	timeTxt.setPosition(window.getSize().x - 150.0f, 40.0f);
	timeTxt.setCharacterSize(18);
	timeTxt.setString("Time: 0");
	timeTxt.setFillColor(sf::Color::Green);

	sf::Clock clk;

	bool inError = false;
	std::string typedString;

	float timePassed{};

	while(window.isOpen())
	{
		sf::Event event;
		timePassed += clk.restart().asSeconds();
		while(window.pollEvent(event))
		{
			char enteredChar = '\0';
			switch(event.type)
			{
				case sf::Event::KeyPressed:
				{
					if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
					}
				}
				break;
				case sf::Event::Closed:
				{
					window.close();
				}
				break;

				case sf::Event::TextEntered:
				{
					if (event.text.unicode <= 128)
					{
						enteredChar = (uint8_t)event.text.unicode;
						if (enteredChar == '\b')
						{
							if (typedString.empty() == false)
								typedString.pop_back();
						}
						else
						{
							typedString += enteredChar;
						}
						typedText.setString(typedString);
					}
				}
				break;
			}
			if (!inError)
			{
				if (enteredChar != targetChar && (isalpha(enteredChar)))
				{
					errors++;
					inError = true;
					std::string error = "Errors: " + std::to_string(errors);
					errorTxt.setString(error);
				}
				else if (enteredChar == targetChar)
				{
 					texts[currentTextIndex].enterColor(index);
					index++;
					if (index == maxSize)
					{
						index = 0;
						typedString = "";
						typedText.setString(typedString);
						GenerateText(generatedWords[currentTextIndex], availableWords);
						texts[currentTextIndex].setString(font, generatedWords[currentTextIndex]);

						currentTextIndex = (currentTextIndex + 1) % texts.size();

						currentTextStr = generatedWords[currentTextIndex];
						maxSize = currentTextStr.size();

					}
					targetChar = currentTextStr[index];
				}
			}
			else
			{
				// Such a bad code :D
				bool flag = true;
				for (int i = 0; i < std::min(typedString.size(), currentTextStr.size()); i++)
				{
					if (currentTextStr[i] != typedString[i])
					{
						inError = true;
						flag = false;
					}
				}
				if (flag)
					inError = false;
			}

			float lastWidth{};
			texts[currentTextIndex].setPosition(whiteSpace.getPosition());
			texts[currentTextIndex].move(window.getSize().x / 2.0f - texts[currentTextIndex].getWidth() / 2.0f, whiteSpace.getSize().y / 3.0f);
			auto startPos = texts[currentTextIndex].getPosition();
			startPos.x += texts[currentTextIndex].getWidth() + 30.0f;
			for (size_t i = (currentTextIndex + 1) % texts.size(), j = 0; j < texts.size() - 1; i = (i + 1) % texts.size(), j++)
			{
				auto& txt = texts[i];
				txt.setPosition(startPos);
				txt.move(lastWidth, 0.0f);
				lastWidth += txt.getWidth() + 30.0f;
			}

			typedText.setPosition(window.getSize().x / 2.0f - typedText.getLocalBounds().width / 2.0f, 100.0f);

		}
		timeTxt.setString("Time: " + std::to_string((uint32_t)timePassed));

		window.clear(sf::Color(64,64,64));
		{
			window.draw(timeTxt);
			window.draw(errorTxt);
			window.draw(typedText);
			window.draw(whiteSpace);

			for(auto& txt : texts)
				window.draw(txt);
		}
		window.display();
	}
	return 0;
}