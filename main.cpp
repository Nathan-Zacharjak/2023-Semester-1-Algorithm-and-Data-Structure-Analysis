#include <iostream>
#include <list>
#include <string>
#include <math.h>

using namespace std;

list<int> schoolAddition(list<int> num1, list<int> num2, int base){
    // Initialise carry and sum
    int carry = 0;
    list<int> sum;

    // For every pair of digits, add them plus the last carry into sum
    while (!(num1.empty() && num2.empty())){
        int digit1 = 0;
        int digit2 = 0;

        if (!num1.empty()){
            digit1 = num1.back();
            num1.pop_back();
        } 
        if (!num2.empty()){
            digit2 = num2.back();
            num2.pop_back();
        }
        // cout << "Digit 1: " << digit1 << " Digit 2: " << digit2 << endl;
        
        int digitSum = digit1 + digit2 + carry;
        // If sum is equal or greater than the base,
        if (digitSum >= base){
            // Make next carry equal 1 and make the sum the modulo of the base
            carry = 1;
            digitSum = digitSum % base;
        } else {
            carry = 0;
        }
        // cout << "Carry: " << carry << " digitSum: " << digitSum << endl;
        sum.push_front(digitSum);
    }

    // After the loop, add the carry to the sum
    if (carry == 1){
        sum.push_front(carry);
    }
    return sum;
}

list<int> schoolSubtractionHelper(list<int> num1, list<int> num2, int base){
    // Initialise carry and sub
    int carry = 0;
    list<int> sub;

    // For every pair of digits, add them plus the last carry into sub
    while (!(num1.empty() && num2.empty())){
        int digit1 = 0;
        int digit2 = 0;

        if (!num1.empty()){
            digit1 = num1.back();
            num1.pop_back();
        } 
        if (!num2.empty()){
            digit2 = num2.back();
            num2.pop_back();
        }

        // Subtract the next numbers and subtract the carry
        int digitSub = digit1 - digit2 - carry;

        // If we're at the front pair of digits,
        // subtract the numbers and subtract the carry, but don't add the base to leave the negative sign
        if (num1.empty() && num2.empty()){
            sub.push_front(digitSub);
            return sub;
        
        } else if (digitSub < 0){
            // if they go negative, add the base, and make the carry 1
            carry = 1;
            digitSub = digitSub + base;
        } else {
            // If not negative, make the carry 0
            carry = 0;
        }

        sub.push_front(digitSub);
    }

    return sub;
}

/*
Subtraction helper function:
1. Check the first number's size, if larger, subtract
2. If equal size, loop through both numbers from the front to the end,
3. If a digit from the first is larger, set subtract bool,
4. If a digit from the first is smaller, set flip subtract bool
5. If did whole loop, completely equal, return a list just containing a single 0
6. Else if subtract, return subtract
7. Else if flip subtract, get subtract, add a negative sign
*/
list<int> schoolSubtraction(list<int> num1, list<int> num2, int base){
    int size1 = num1.size();
    int size2 = num2.size();
    list<int> sub;
    bool negative = false;

    if (size1 > size2){
        sub = schoolSubtractionHelper(num1, num2, base);
    } else if (size1 < size2){
        sub = schoolSubtractionHelper(num2, num1, base);
        negative = true;
    } else {
        list<int>::iterator num2It = num2.begin();
        
        for (list<int>::iterator num1It = num1.begin(); num1It != num1.end(); num1It++){
            int digit1 = *num1It;
            int digit2 = *num2It;
            
            if (digit1 > digit2){
                sub = schoolSubtractionHelper(num1, num2, base);
                break;
            } else if (digit1 < digit2){
                sub = schoolSubtractionHelper(num2, num1, base);
                negative = true;
                break;
            }

            num2It++;
        }
        // If lopped through whole lists, both numbers are equal, return 0
        if (sub.size() == 0){
            sub.push_front(0);
            return sub;
        }
    }

    // Remove leading 0s
    while (sub.front() == 0){
        sub.pop_front();
        // Unless the result is a single 0
        if (sub.size() == 1){
            break;
        }
    }
    // If negative, make the first number negative
    if (negative){
        sub.front() = -sub.front();
    }
    
    return sub;
}

list<int> schoolPartialProduct(list<int> num1, int num2, int base){
    // Initialise base number and carry lists,
    // base numbers are the right digits of multiplication
    // carry numbers are the left digits
    list<int> baseNumbers;
    list<int> carryNumbers;

    // Loop until the last digit of the first number is multiplied
    while (!num1.empty()){
        // Multiply the first digit of the first number by the second
        int digit = num1.back();
        num1.pop_back();
        int prod = digit * num2;

        // Find the modulo of the product and the base, and store that as the first digit of the base number list
        int remainder = prod % base;
        baseNumbers.push_front(remainder);

        // If the number is larger than or equal to the base, store the carry in the carry list,
        // else add 0 to the carry list
        if (prod >= base){
            int carry = (prod - remainder)/base;
            carryNumbers.push_front(carry);
        } else {
            carryNumbers.push_front(0);
        }
    }

    // This is the addition step where we add the carries together with the base numbers
    // to get the actual partial product
    list<int> partialProduct;
    int additionCarry = 0;

    // Make the back number of the base number list the first digit of the partial product
    int firstDigit = baseNumbers.back();
    partialProduct.push_front(firstDigit);
    baseNumbers.pop_back();

    while (!baseNumbers.empty()){
        // Add the next back number of the base list with the first back number of the carry list
        int sum = baseNumbers.back() + carryNumbers.back() + additionCarry;
        baseNumbers.pop_back();
        carryNumbers.pop_back();

        // If larger than the base,
        // make additionCarry = 1 and set next digit to the modulo of the sum and the base
        if (sum >= base){
            additionCarry = 1;
        } else {
            additionCarry = 0;
        }

        int digit = sum % base;
        partialProduct.push_front(digit);
    }
    
    // Add the last carry digit to the front of the partial product list if not 0 or there is an addition carry
    int lastDigit = carryNumbers.back() + additionCarry;
    if (lastDigit != 0){
        partialProduct.push_front(lastDigit);
    }

    return partialProduct;
}

list<int> schoolMultiplication(list<int> num1, list<int> num2, int base){
    // Set the initial product to 0
    list<int> product;
    product.push_front(0);
    int loopsComplete = 0;

    // Loop until the last digit in the second number is done
    while (!num2.empty()){
        // Get the partial product of the first digit of num2
        list<int> partialProduct = schoolPartialProduct(num1, num2.back(), base);
        num2.pop_back();

        // For however many loops have gone before, add zeros to the end of the partial product
        for (int i = 0; i < loopsComplete; i++){
            partialProduct.push_back(0);
        }

        // Add the current product to the partial product using school addition
        product = schoolAddition(product, partialProduct, base);
        
        loopsComplete++;
    }

    return product;
}

list<int> karatsubaMultiplication(list<int> A, list<int> B, int base){
    // For the input numbers A and B, if either are single digits, perform school multiplication on them and return
    long unsigned int An = A.size();
    long unsigned int Bn = B.size();

    if (An == 1 || Bn == 1){
        return schoolMultiplication(A, B, base);
    }

    // Make the input A and B have equal digits by adding leading 0s
    long unsigned int n = Bn;
    if (An > Bn){
        n = An;
    }
    if (n == An){
        while (B.size() < n){
            B.push_front(0);
        }
    } else {
        while (A.size() < n){
            A.push_front(0);
        }
    }
    
    // Else, find K for both of them using the number of digits in either number
    // N = the number of digits in the larger of the 2 numbers
    // K = ceil(n/2)
    int k = ceil(n/2.0);

    // Make a0 and b0 equal to the last K digits of a and b, make a1 and b1 equal to the rest
    list<int> A0;
    list<int> B0;
    int loopCount = k;
    for (int i = 0; i < loopCount; i++){
        A0.push_front(A.back());
        A.pop_back();
        B0.push_front(B.back());
        B.pop_back();
    }

    list<int> A1;
    list<int> B1;
    while (!A.empty()){
        A1.push_front(A.back());
        A.pop_back();
        B1.push_front(B.back());
        B.pop_back();
    }

    // Call the function again 3 times, once for each P inputting them as the new a and b,
    //    to get the values of P0, P1 and P2
    list<int> P1a = schoolAddition(A1, A0, base);
    list<int> P1b = schoolAddition(B1, B0, base);
    list<int> P0 = karatsubaMultiplication(A0, B0, base);
    list<int> P1 = karatsubaMultiplication(P1a, P1b, base);
    list<int> P2 = karatsubaMultiplication(A1, B1, base);

    // Calculate and return the big AxB formula result, subbing in all the calculated values,
    // using school addition, subtraction and multiplication
    list<int> P1minusP2 = schoolSubtraction(P1, P2, base);
    list<int> P1minusP2minusP0 = schoolSubtraction(P1minusP2, P0, base);

    // base^(ceil(n/2)) = Add K 0s to the end of the multiplied numbers, from the K calculated earlier
    for (int i = 0; i < 2*k; i++){
        P2.push_back(0);
    }
    for (int i = 0; i < k; i++){
        P1minusP2minusP0.push_back(0);
    }
    // list<int> P2Print = P2;
    // list<int> minusPrint = P1minusP2minusP0;
    // list<int> P1Print = P1;
    // cout << "P2: ";
    // while (!P2Print.empty()){
    //     cout << P2Print.front();
    //     P2Print.pop_front();
    // }
    // cout << " Mid: ";
    // while (!minusPrint.empty()){
    //     cout << minusPrint.front();
    //     minusPrint.pop_front();
    // }
    // cout << " P1: ";
    // while (!P1Print.empty()){
    //     cout << P1Print.front();
    //     P1Print.pop_front();
    // }
    // cout << endl;

    return schoolAddition(P2, schoolAddition(P1minusP2minusP0, P0, base), base);
}

int main(int argc, char const *argv[]){
    // Get each input number into arrays of integers, and the base
    list<int> num1;
    list<int> num2;
    int base = 0;

    // i=1 to skip the first argument which is just ./main.out
    for (int i = 1; i < argc; i++){
        string input = argv[i];

        if (i == 3){
            base = stoi(input);
            break;
        }

        for (int j = 0; j < (int)input.length(); j++){
            // Doing the usual magic trick of
            // taking away 0's ASCII code from a number char to convert it into an int
            int digit = input.at(j) - '0';

            if (i == 1){
                num1.push_back(digit);
            } else if (i == 2) {
                num2.push_back(digit);
            }
        }
    }

    list<int> sum = schoolAddition(num1, num2, base);
    list<int> product = karatsubaMultiplication(num1, num2, base);
    // Print the results!
    while (!sum.empty()){
        cout << sum.front();
        sum.pop_front();
    }
    cout << " ";

    while (!product.empty()){
        cout << product.front();
        product.pop_front();
    }
    
    // Print "0" as the division result as the assignment asks
    cout << " 0" << endl;
    
    return 0;
}