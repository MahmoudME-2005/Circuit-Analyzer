#include <iostream>
#include <string>

using namespace std;
/*Important notes:
* student name: Mahmoud Ehab Moheb.
* student ID: 2301085.

1) at first i used a global variable as the iterator variable i will be the same across all for loops across all functions as to avoid
looping on the same character twice to prevent errors in calculations and inefficiency in the code.
but then switched to just passing the iterator variable between the functions by reference after learning it in the lecture.

2) Inputs like "s 1 1 e p 1 1 e" has no meaning so they are not accepted and an error message will be printed "Wrong Description".
*/

float series_solver(string des,int &i);//Declaration for the series solver function.

float parallel_solver(string des, int &i)//the function that will be responsible for solving the parallel connected circuits.
{
    float parallel_result = 0,temporary;
    string sub_string = "";
    int counter = 0;

    for(;des[i]!='e';i++)//loops until it meets the character e(the circuit description terminator).
    {
        if((des[i]>=48 && des[i]<=57) || des[i] == '.')//checks for numeric values and accepts floating point numbers.
        {
            sub_string += des[i];
        }
        else if(des[i] == 's' || des[i] == 'S')//checks for any nested loops connected in series.
        {
            if(counter == 0)//checks if it's the first number in the connection or not.
            {
                parallel_result = series_solver(des,i+=2);
                counter++;
            }
            else//if it's not the first number in the connection.
            {
                temporary = series_solver(des,i+=2);
                parallel_result = 1/((1/parallel_result)+(1/temporary));//the formula that calculates the parallel result and handles if any zero valued resistors were inserted and deals with them as short circuits.
                counter++;
            }
        }
        else if(des[i] == 32)//checks if the character is an empty space or not.
        {
            if(sub_string != "")//checks if the substring is empty or not.
            {
                if(counter == 0)//checks if it's the first number in the connection or not.
                {
                    parallel_result = stof(sub_string);
                    sub_string = "";
                    counter++;
                }
                else//if it's not the first number in the connection.
                {
                    parallel_result = 1/((1/(parallel_result))+(1/(stof(sub_string))));//the formula that calculates the parallel result and handles if any zero valued resistors were inserted and deals with them as short circuits.
                    sub_string = "";
                    counter++;
                }
            }
            else//skips if the substring is empty.
            {
                continue;
            }
        }
        else if(des[i] == 'p' || des[i] == 'P')//calls it self to solve a nested parallel circuit(recursion).
        {
            if(counter == 0)
            {
                parallel_result = parallel_solver(des,i+=2);
                counter++;
            }
            else
            {
                temporary = parallel_solver(des,i+=2);
                parallel_result = 1/((1/parallel_result)+(1/temporary));
                counter++;
            }
        }
        else//if the character is an invalid input.
        {
            cout<<"Wrong Description";
            exit(0);
        }
    }
    if(counter < 2)//checks if the parallel connection has more than two resistors or not.
    {
        cout<<"Incorrect Input";
        exit(0);
    }
    else
    {
        return parallel_result;//returns the solution of the parallel connected series.
    }
}

float series_solver(string des,int &i)//the function that's gonna be responsible for solving circuits connected in series.
{
    int counter = 0;
    float series_result = 0;
    string sub_string = "";
    for(;des[i] != 'e';i++)//loops until it meets the character e(the circuit description terminator).
    {
        if((des[i] >= 48 && des[i] <= 57) || des[i] == '.')//checks for numeric values and accepts floating point numbers.
        {
            sub_string += des[i];
        }
        else if(des[i] == 'p' || des[i] == 'P')//checks for any nested loops connected in parallel.
        {
            series_result += parallel_solver(des,i+=2);//calls the parallel solver function to solve a nested loop connected in parallel.
            counter++;
        }
        else if(des[i] == 32)//checks if the character is an empty space or not.
        {
            if(sub_string != "")//checks if the substring is empty or not.
            {
                series_result += stof(sub_string);
                sub_string = "";
                counter++;
            }
            else//skips if the substring is empty.
            {
                continue;
            }
        }
        else if(des[i] == 's' || des[i] == 'S')//calls it self to solve a series nested circuit(recursion).
        {
            series_result += series_solver(des,i+=2);
            counter++;
        }
        else//if the character is an invalid input.
        {
            cout<<"Wrong Description";
            exit(0);
        }
    }
    if(counter<1)//checks if the series connection has more than one resistor or not.
    {
        cout<<"Incorrect Input";
        exit(0);
    }
    else
    {
        return series_result;//returns the solution of the series connected circuit.
    }
}

float circuit_analyzer(string des)//the function that manages the two circuit solving functions and returns the solution of the whole circuit.
{
    int len = des.length();
    float result = 0;
    int i = 0;

    for(;i<len;i++)//Loops on the input till the end
    {
        if((des[i] == 's' || des[i] == 'S') && result == 0)//Checks for circuits connected in series.
        {
            result = series_solver(des,i+=2);//calls the series solver function to solve a circuit connected in series.
        }
        else if((des[i] == 'p' || des[i] == 'P') && result == 0)//Checks for circuits connected in parallel.
        {
            result = parallel_solver(des,i+=2);//calls the parallel solver function to solve a circuit connected in parallel.
        }
        else if(des[i] == 32)//skips spaces as to not trigger any error if the user enters an extra space by mistake in the circuit description.
        {
            continue;
        }
        else//if the character is an invalid input.
        {
            cout<<"Wrong Description";
            exit(0);
        }
    }
    return result;//returns the solution to circuit.
}

int main()
{
    string description;
    getline(cin,description);
    float x = circuit_analyzer(description);
    cout<<"The total resistance = "<<x;
}
