#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <unistd.h>

bool loadStrings( std::string fileName, std::vector<std::string>& strings )
{
    std::fstream file;
    file.open( fileName );
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

    char* source;
    int copies = 1;
    sf::Font font;
    sf::Text text;
    sf::Texture bonusImage;
    sf::Sprite bonusSprite;
    sf::Image screen;

    //get args
    while( ( arg = getopt( argc, argv, "i:s:b:f:" ) ) != -1 )
    {
        switch( arg )
        {
            case 'i':
            {
                std::string temp = optarg;
                copies = std::stoi( temp );
            
                break;
            }
            case 's':
                source = optarg;

                break;
            case 'b':
                if( !bonusImage.loadFromFile( optarg ) )
                    return 1;
                bonusSprite.setTexture( bonusImage );
                bonusSprite.setPosition( 200, 201 );

                break;
            case 'f':
            {
                std::string fontString = optarg;
                if( !font.loadFromFile( fontString ) )
                    return 1;
                
                text.setFont( font );
                text.setCharacterSize( 13 );
                text.setFillColor( sf::Color::Black );

                break;
            }
            case '?':
                if( optopt == 'i' || optopt == 's' || optopt == 'b' || optopt == 'f' )
                    std::cout << "Option requires an argument." << std::endl;
                else if( isprint( optopt ) )
                    std::cout << "Unkown option: " << optopt << std::endl;
                else
                    std::cout << "Unkown option character" << std::endl;
                return 1;

                break;
        }
    }


    srand( time( NULL ) );

    std::vector<std::string> strings;
    if( !loadStrings( source, strings ) )
        return 0;
    
    strings.shrink_to_fit();
    int len = strings.size()-1;
    
    for( int k = 0; k < copies; k++ ) //copy loop
    {
        bool wasDrawn[len] = { false };
        int whichLine[len] = { 0 };
        int number;
        
        //random draw
        for( int i = 0; i < len; i++ )
        {
            do
            {
                number = std::rand() % len;
                whichLine[i] = number;
            } while( wasDrawn[number] == true );
            wasDrawn[number] = true;
        }

        sf::RenderTexture window;
        if( !window.create( 500, 500 ) )
            return 1;
        
        window.clear( sf::Color::White );

        //draw lines
        sf::VertexArray lines( sf::LineStrip, 2 );
        lines[0].color = sf::Color::Black;
        lines[1].color = sf::Color::Black;
            
        for( int i = 1; i < 5; i++ ) //x
        {
            lines[0].position = sf::Vector2f( i*100, 0 );
            lines[1].position = sf::Vector2f( i*100, 500 );

            window.draw( lines );
        }   
        for( int i = 1; i < 5; i++ ) //y
        {
            lines[0].position = sf::Vector2f( 0, i*100 );
            lines[1].position = sf::Vector2f( 500, i*100 );

            window.draw( lines );
        }   

        //draw text
        int y = 0;
        int z = 0;
        int bonusTile = 12;
        int tileCount = 25;
        for( int i = 0; i < tileCount; i++, z++ )
        {
            if( i != bonusTile )
            {
                text.setString( strings[ whichLine[i] ] );
            }else
            {
                text.setString( "\n*" );
                window.draw( bonusSprite );
            }
            
            int a = round( 50+z*100 - text.getGlobalBounds().width / 2 ); //center text
            int b = y*100;
            text.setPosition( a, b ); 

            if( z % 4 == 0 && z != 0 )
            {
                z = -1;
                y++;
            }
            
            window.draw( text );
        }

        //save
        screen = window.getTexture().copyToImage();
        screen.flipVertically();
        std::string outputName = "out";
        outputName = outputName + std::to_string(k+1) + ".png";
        screen.saveToFile( outputName ); 
    } //
    std::cout << "Done!" << std::endl;
    return 0;
}
