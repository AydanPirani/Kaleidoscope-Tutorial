#pragma once

#include <string>

enum Token
{
    // file end
    tok_eof = -1,

    // commands to run
    tok_define = -2,
    tok_external = -3,

    tok_identifier = -4,
    tok_number = -5
};

static std::string identifier_string;
static double num_val;

// gets the next token in the program
static int get_token()
{
    static int last_char = ' ';

    // Ignore all whitespace, and keep reading
    while (isspace(last_char))
    {
        last_char = getchar();
    }

    // Identifiers: any strings that are not whitespace separated
    if (isalpha(last_char))
    {
        identifier_string = last_char;

        // Read the next character and get the output
        while (isalnum((last_char = getchar())))
        {
            identifier_string += last_char;
        }

        // Define for the first function
        if (identifier_string == "define")
        {
            return tok_define;
        }

        if (identifier_string == "external")
        {
            return tok_external;
        }

        return tok_identifier;
    }

    // At a number - create a string, and then convert to decimal
    if (isdigit(last_char))
    {
        std::string number_string;
        bool seen_decimal;

        // Convert string to a decimal
        do
        {
            number_string += last_char;
            last_char = getchar();
            seen_decimal = (last_char == '.');
        } while (isdigit(last_char) || (last_char == '.' && !seen_decimal));

        // Convert number string to decimal, and output it
        num_val = strtod(number_string.c_str(), 0);
        return num_val;
    }

    // Comment for the rest of the line
    // TODO: Add in multi-line commends
    if (last_char == '#')
    {
        do
        {
            last_char = getchar();
        } while (last_char != EOF && last_char != '\n' && last_char != '\r');

        // Not at end of file - returns the next token
        if (last_char != EOF)
        {
            return get_token();
        }
    }

    // End of file - simply return the end of file token
    if (last_char == EOF)
    {
        return tok_eof;
    }

    // Char is not "special" - direcly return the current char
    int this_char = last_char;
    last_char = getchar();
    return this_char;
}