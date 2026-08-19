// This work was done by Alexandra Yoder, Lauren Petty, and Kamryn Klik on 4/29/2026

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

// function prototypes
int displayMenu();
double validInput(string prompt, bool allowZero);
double calculateRevenue(double price, int quantity, double taxRate);
double calculateGrowthRate(int previousQuantity, int currentQuantity, double DEFAULT_GROWTH_RATE);
void displayProductSummary(string productName, double currentRevenue, double predictedQuantity, double predictedRevenue, double growthRate, bool isPromo, double promoPrice);

int main ()
{
    ofstream outFile("sales_report.txt");

    // Constants
    const double TAX_RATE = 0.07;
    const double DEFAULT_GROWTH_RATE = 5.0;

    // Variables
    string productName;
    double price;
    double growthRate;
    double currentRevenue;
    double predictedQuantity;
    double predictedRevenue;
    double averageGrowthRate;
    double averageRevenue;
    int previousQuantity;
    int currentQuantity;
    char runPromo;
    bool isPromo;
    double discountPercent;
    double promoPrice;

    // Summary Trackers
    vector<string> productHistory;
    string highestProduct;
    string lowestProduct;
    string highestGrowthProduct;
    string lowestGrowthProduct;
    double highestRevenue;
    double lowestRevenue;
    double highestGrowthRate;
    double lowestGrowthRate;
    double totalGrowthRate = 0;
    double totalRevenue = 0;
    int productCount = 0;

    int choice;

    cout << fixed << setprecision(2);
    outFile << fixed << setprecision(2);

    cout << "Welcome to the Grocery Store Sales Predictor" << endl;
    cout << "This program collects sales data and uses it to predict future sales" << endl;

    while (true)  // Loop for repeating process
    {
	choice = displayMenu(); // function 1

        if (choice == 1)
        {
            // User input
            cout << " " << endl;
            cout << "Enter product name: " << endl;
            getline(cin, productName);

            productHistory.push_back(productName);

            // Using validInput (function 2) for price and quantities
            price = validInput("Enter price of product: $", false);
            previousQuantity = (int)validInput("Enter quantity sold LAST month: ", true);
            currentQuantity = (int)validInput("Enter quantity sold THIS month: ", true);

            // Calculating growth rate (function 4)
            growthRate = calculateGrowthRate(previousQuantity, currentQuantity, DEFAULT_GROWTH_RATE);

            // Predictions depend on whether the product with be discounted next month
            cout << "Will this product be on promotion next month? (y/n): ";
            cin >> runPromo;

            if (runPromo == 'y' || runPromo == 'Y') {
                isPromo = true;
                cout << "We will assume a 10% boost in sales due to the promotion" << endl;
                cout << "Enter discount percentage (e.g., 20 for 20%): ";
                cin >> discountPercent;
                promoPrice = price * (1 - (discountPercent / 100.0));

                // Prediction: regular growth + 10% extra boost from the sale
                predictedQuantity = (currentQuantity * (1 + growthRate / 100)) * 1.10;
                predictedRevenue = calculateRevenue(promoPrice, (int)predictedQuantity, TAX_RATE);
            } else {
                isPromo = false;
                promoPrice = price; // No change
                predictedQuantity = currentQuantity * (1 + growthRate / 100);
                predictedRevenue = calculateRevenue(price, (int)predictedQuantity, TAX_RATE);
            }

            cout << "Press Enter to continue..." << endl;
            cin.ignore(1000, '\n');
            cin.get();

	    // Calculate revenue with function 3
            currentRevenue = calculateRevenue(price, currentQuantity, TAX_RATE);

            totalRevenue += currentRevenue;
            totalGrowthRate += growthRate;
            productCount++;

            // Predicted sales calculations
            predictedQuantity = currentQuantity * (1 + growthRate / 100);
            predictedRevenue = calculateRevenue(price, (int)predictedQuantity, TAX_RATE); // using function 3

	    // Display per-product summary using function 5
	    displayProductSummary(productName, currentRevenue, predictedQuantity, predictedRevenue, growthRate, isPromo, promoPrice);

          // initializing using first product entered
	  if (productCount == 1) {
          highestRevenue = currentRevenue; // first product is highest by default
          lowestRevenue = currentRevenue; // first product is lowest by default
          highestProduct = productName;
          lowestProduct = productName;

          highestGrowthRate = growthRate; // first product is highest by default
          lowestGrowthRate = growthRate; // first product is lowest by default
          highestGrowthProduct = productName;
          lowestGrowthProduct = productName;
	  }

          else { // Revenue comparisons to update highest and lowest selling products

          if (currentRevenue > highestRevenue) { // check if new product has higher revenue
          highestRevenue = currentRevenue; // update highest revenue value
          highestProduct = productName; // update highest revenue product
          }

          if (currentRevenue < lowestRevenue) { // check if new product has lower revenue
          lowestRevenue = currentRevenue; // update lowest revenue value
          lowestProduct = productName; // update lowest revenue product
          }
	  // Growth rate updates
          if (growthRate > highestGrowthRate) { //same idea as above for revenue
          highestGrowthRate = growthRate;
          highestGrowthProduct = productName;
          }
          if (growthRate < lowestGrowthRate) {
          lowestGrowthRate = growthRate;
          lowestGrowthProduct = productName;
          }
	}

            cout << "Press Enter to continue..." << endl;
            cin.ignore();

	    // Write to output file
            outFile << "----------------------" << endl;
            outFile << "Product: " << productName << (isPromo ? " (PROMO)" : "") << endl;
            outFile << "Revenue: $" << currentRevenue << endl;
            outFile << "Growth Rate: " << growthRate << "%" << endl;
            outFile << "Predicted Quantity: " << predictedQuantity << endl;
            outFile << "Predicted Revenue: $" << predictedRevenue << endl;

	    // Compare predicted vs current
            if (predictedRevenue > currentRevenue)
                cout << "Sales are expected to increase." << endl;
            else if (predictedRevenue < currentRevenue)
                cout << "Sales are expected to decrease." << endl;
            else
                cout << "Sales are expected to remain the same." << endl;
 }

else if (choice == 2)
       {
         if (productCount == 0) // If they initially enter choice 2, they must choose choice 1
         {
         cout << "Invalid choice. You must enter at least one product first." << endl;
         continue;
         }
         else //They choose choice 2 after entering at least one product
         {
         break; // exits loop, goes to summary
         }
}
else if (choice == 3) // View Product List
        {
            if (productHistory.empty()) {
                cout << "No products have been entered yet." << endl;
            } else {
                cout << "\n--- CURRENT PRODUCT LIST ---" << endl;
                for (int i = 0; i < productHistory.size(); i++) {
                    cout << i + 1 << ". " << productHistory[i] << endl;
                }
                cout << "----------------------------" << endl;
            }
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }


    // Summary calculations and predictions
    averageRevenue = totalRevenue / productCount;
    averageGrowthRate = totalGrowthRate / productCount;

    cout << " " << endl;
    cout << "Cumulative Sales Summary" << endl;

    cout << "Total Revenue: $" << totalRevenue << endl;
    cout << "Average Revenue per Product this Month: $" << averageRevenue << endl;
    cout << " " << endl;

    cout << "The average growth rate of the products is: " << averageGrowthRate << "%" << endl;
    if (averageGrowthRate > 10)
        cout << "Indicates strong performance." << endl;
    else if (averageGrowthRate >= 0)
        cout << "Indicates moderate performance." << endl;
    else
        cout << "Indicates poor performance." << endl;

    cout << " " << endl;

    cout << "Highest Selling Product: " << highestProduct
         << " ($" << highestRevenue << ")" << endl;

    cout << "Lowest Selling Product: " << lowestProduct
         << " ($" << lowestRevenue << ")" << endl;

    cout << "Highest Growth Product: " << highestGrowthProduct
         << " (" << highestGrowthRate << "%)" << endl;

    cout << "Lowest Growth Product: " << lowestGrowthProduct
         << " (" << lowestGrowthRate << "%)" << endl;

    cout << " " << endl;

    if (averageGrowthRate > 0)
        cout << "The store is expected to see an increase in sales." << endl;
    else if (averageGrowthRate == 0)
        cout << "The store is expected to stay the same." << endl;
    else
        cout << "The store is expected to see a decrease in sales." << endl;

    cout << " " << endl;
    cout << "Thank you for using the program!" << endl;

    // Write summary to output file
    outFile << "---OVERALL SUMMARY---" << endl;
    outFile << "Average Revenue: $" << averageRevenue << endl;
    outFile << "Average Growth Rate: " << averageGrowthRate << "%" << endl;
    outFile << "Highest Selling Product: " << highestProduct
            << " ($" << highestRevenue << ")" << endl;
    outFile << "Lowest Selling Product: " << lowestProduct
            << " ($" << lowestRevenue << ")" << endl;

    outFile.close();

    return 0;
}


// Function definitions

// 1. Menu choice
int displayMenu() {
     int choice;
     cout << " " << endl;
     cout << "Menu:" << endl;
     cout << "1. Enter a new product" << endl;
     cout << "2. Finish and view summary" << endl;
     cout << "3. View product list" << endl;
     cout << "Enter your choice: ";
     cin >> choice;
     cin.ignore(1000, '\n');
     return choice;
}

// 2. Validation function
double validInput(string prompt, bool allowZero) {
     double value;
     cout << prompt << endl;
     cin >> value;

     if (allowZero) {
        while (value < 0) {
            cout << "Please enter a valid quantity: " << endl;
            cin >> value;
        }
    } else {
        while (value <= 0) {
            cout << "Please enter a valid price: " << endl;
            cin >> value;
        }
    }
    return value;
}

// 3. Returns calculated revenue
double calculateRevenue(double price, int quantity, double taxRate) {
     double revenue = price * quantity;
     return revenue + (revenue * taxRate);
}

// 4. Returns growth rate precentage
double calculateGrowthRate(int previousQuantity, int currentQuantity, double DEFAULT_GROWTH_RATE) {
     if (previousQuantity == 0) {
        cout << "No previous sales data. Using default growth rate of " << DEFAULT_GROWTH_RATE << "%" << endl;
        return DEFAULT_GROWTH_RATE;
     }
     double rate = (currentQuantity - previousQuantity) / (double)previousQuantity * 100;
     cout << "Growth rate for the product is " << rate << "%" << endl;
     return rate;
}

// 5. Product summary
void displayProductSummary(string productName, double currentRevenue, double predictedQuantity, double predictedRevenue, double growthRate, bool isPromo, double pPrice) {
    cout << "Current Sales Info" << endl;
    cout << "Product: " << productName << endl;
    cout << "Revenue (with tax): $" << currentRevenue << endl;
    if (isPromo) {
        cout << "PROMOTION APPLIED: Next month's price will be $" << pPrice << endl;
    }

    cout << "Press Enter to continue..." << endl;
    cin.ignore(1000, '\n');

    cout << "Predicted Sales" << endl;
    cout << "Predicted Quantity: " << predictedQuantity << endl;
    cout << "Predicted Revenue: $" << predictedRevenue << endl;
}
