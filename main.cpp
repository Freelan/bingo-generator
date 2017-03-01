#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <unistd.h>

bool loadStrings( std::string fileName, std::vector<std::string>& strings )
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
    	std::cout << "Error loading file." << std::endl;
    	return false;
    }
}

int main( int argc, char *argv[] )
{
    int arg;
    opterr = 0;

    char* source = NULL;
    int copies = 1;

    while( ( arg = getopt( argc, argv, "i:s:" ) ) != -1 )
    {
        switch( arg )
        {
            case 'i':
                {
                    std::string temp = optarg;
                    copies = std::stoi( temp );
                }
                break;
            case 's':
                source = optarg;
                break;
            case '?':
                if( optopt == 'i' )
                    std::cout << "Option requires an argument." << std::endl;
                else if( isprint( optopt ) )
                    std::cout << "Unkown option." << std::endl;
                else
                    std::cout << "Unkown option character" << std::endl;
                return 1;
                break;
        }
    }


    srand( time( NULL ) );
    sf::Image screen;

    std::vector<std::string> strings;
    if( !loadStrings( source, strings ) )
        return 0;
    
    strings.shrink_to_fit();
    int len = strings.size()-1;
    //std::cout << len << std::endl;
    
    
    for( int k = 0; k < copies; k++ )
    {
        bool wasDrawn[len] = { false };
        int whichLine[len] = { 0 };
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

        sf::RenderWindow window( sf::VideoMode(500, 500), "Bingo Generator" );
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
                if( i != 12 )
                {
                    text.setString( strings[ whichLine[i] ].c_str() );
                }else
                {
                    text.setString( "\nBONUS" );
                }

                text.setPosition( 50+z*100 - text.getGlobalBounds().width / 2, y*100 );

                if( z % 4 == 0 && z != 0 )
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
        std::string outputName = "out";
        outputName = outputName + std::to_string(k+1) + ".png";
        screen.saveToFile( outputName.c_str() ); 
    
    }

    return 0;
}
