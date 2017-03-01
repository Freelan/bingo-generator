#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>

bool loadStrings( std::string fileName, std::vector& strings )
{
    std::fstream file;
    file.open( fileName.c_str() );
    if( file.good() )
    {
        while( !file.eof() )
        {
        	std::string temp;
            getline( file, temp, ';' );
            strings.push_back( temp );
        }
        file.close();
        return true;
    }else
    {
    	std::cout << "Error loading file." << std:endl;
    	return false;
    }
}

int main( int argc, char *argv[] )
{
	for( int i = 0; i < argc; i++ )
	{
		if( argv[i][0] == '-' )
		
	
	
    srand( time( NULL ) );
    sf::RenderWindow window( sf::VideoMode(500, 500), "Bingo Generator" );
    sf::Image screen;

    std::vector<std::string> strings;
    if( !loadStrings( "smieszne.txt", strings ) )
        return 0;
    int len = strings.capacity();
    
    bool wasDrawn[len] = { false };
    int whichLine[len];
    int number;
    
    for( int i = 0; i < len; i++ )
    {
        do
        {
            number = std::rand() % len;
            whichLine[i] = number;
        } while( wasDrawn[number] == true );
        wasDrawn[number] = true;
    }

    while( window.isOpen() )
    {
        sf::Event event;
        while( window.pollEvent( event ) )
        {
            if( event.type == sf::Event::Closed )
                window.close();
        }
        window.clear( sf::Color::White );
        
        //draw lines
        sf::VertexArray lines( sf::LineStrip, 2 );
        lines[0].color = sf::Color::Black;
        lines[1].color = sf::Color::Black;
        
        for( int i = 1; i < 5; i++ )
        {
            lines[0].position = sf::Vector2f(i*100, 0);
            lines[1].position = sf::Vector2f(i*100, 500);

            window.draw( lines );
        }   
        for( int i = 1; i < 5; i++ )
        {
            lines[0].position = sf::Vector2f(0, i*100);
            lines[1].position = sf::Vector2f(500, i*100);

            window.draw( lines );
        }   

        //draw text
        sf::Font font;
        if( !font.loadFromFile( "DejaVuSansMono.ttf" ) )
        {
            return 0;
        }

        sf::Text text;
        text.setFont( font );
        text.setCharacterSize( 12 );
        text.setFillColor( sf::Color::Black );
        
        int y = 0;
        int z = 0;
        for( int i = 0; i < 29; i++, z++ )
        {
            if( i != 14 )
            {
                text.setString( strings[ whichLine[i] ].c_str() );
            }else
            {
                text.setString( "        BONUS" );
            }
            text.setPosition( z*100, y*100 );

            if( z % 5 == 0 && z != 0 )
            {
                z = -1;
                y++;
            }
            
            window.draw( text );
        }
        window.display();
        screen = window.capture();
    }
    
    screen.flipVertically();
    screen.saveToFile( "3es22t.png" ); 

    return 0;
}