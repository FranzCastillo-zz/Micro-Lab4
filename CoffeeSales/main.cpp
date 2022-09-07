using namespace std;

#include <iostream>

// INITIAL AMOUNTS OF PRODUCTS AND MONTHS
const int nProducts = 8;
const int nMonths = 2;

void separator(){
    cout << "-----------------------------------------------------\n";
}

// CONSTANT PART OF THE PRODUCT
struct BaseProduct{
    string productName;
    double price;
    double cost;
};

// PRODUCT WITH IT'S VARIABLE PARTS
struct Product{
    BaseProduct baseProduct;
    int unitsSold;
    double sales;
    double utility;
};

// MONTH
struct Month{
    string monthName;
    Product products[nProducts];
    double totalSales;
    double utility;
    double variableCosts;
};
void createMonthSales(), printReport(Month* month), analyzeMonthData(Month* month);
void* analyzeProductData(void *args);
Month months[nMonths];

int main() {
    // Initializes month values for each product sold
    createMonthSales();

    // Creates the Threads
    pthread_t threads[nProducts * nMonths];

    // Initializes the threads
    int threadPos = 0;
    for(auto & month : months){
        // Sends each product of each month to a single thread
        for(auto & product : month.products){
            pthread_create(&threads[threadPos++], nullptr, &analyzeProductData, (void*) &product);
        }
    }
    // Waits for all the threads to finish its execution
    for(auto thread : threads){
        pthread_join(thread, nullptr);
    }
    // Prints the month data
    for(Month month : months){
        analyzeMonthData(&month);
        printReport(&month);
    }
    return 0;
}

/**
 * Prints the month values based on the products sold
 * @param month The month with te values to print
 */
void createMonthSales(){
    BaseProduct chocolateCake{"Pastel de Chocolate", 60.0, 20.0};
    BaseProduct whiteMocha{"White Mocha", 32.0, 19.2};
    BaseProduct american{"Cafe Americano", 22.0, 13.2};
    BaseProduct latte{"Latte", 24.0, 17.2};
    BaseProduct toffee{"Toffee Coffee", 28.0, 20.1};
    BaseProduct cappuccino{"Cappuccino 8onz", 24.0, 17.2};
    BaseProduct smores{"S'mores latte", 32.0, 23.0};
    BaseProduct toasted{"Cafe tostado molido", 60.0, 20.0};

    // JULY DATA
    months[0].monthName = "Julio";
    months[0].products[0] = {chocolateCake, 300};
    months[0].products[1] = {whiteMocha, 400};
    months[0].products[2] = {american, 1590};
    months[0].products[3] = {latte, 200};
    months[0].products[4] = {toffee, 390};
    months[0].products[5] = {cappuccino, 1455};
    months[0].products[6] = {smores, 800};
    months[0].products[7] = {toasted, 60};
    months[0].variableCosts = 45640.0;
    // AUGUST DATA
    months[1].monthName = "Agosto";
    months[1].products[0] = {chocolateCake, 250};
    months[1].products[1] = {whiteMocha, 380};
    months[1].products[2] = {american, 800};
    months[1].products[3] = {latte, 250};
    months[1].products[4] = {toffee, 600};
    months[1].products[5] = {cappuccino, 1200};
    months[1].products[6] = {smores, 1540};
    months[1].products[7] = {toasted, 15};
    months[1].variableCosts = 40590.0;
}

/**
 * Check's the month data and stores it
 * @param month the month structure
 */
void analyzeMonthData(Month* month){
    double totalSales = 0;
    double totalUtility = 0;
    for(auto product : month->products){
        totalSales += product.sales;
        totalUtility += product.utility;
    }
    month->totalSales = totalSales;
    month->utility = totalUtility - month->variableCosts;
}

/**
 * Prints the monthly report to the console
 * @param month The month structure with the data contained in it
 */
void printReport(Month* month){
    separator();
    cout << "Reporte del mes " << month->monthName << endl;
    separator();
    cout << "Ventas por producto:\n" << endl;
    for(auto product : month->products){
        cout << product.baseProduct.productName << " Q." << product.sales << endl;
    }
    separator();
    cout << "Utilidad por producto:\n" << endl;
    for(auto product : month->products){
        cout << product.baseProduct.productName << " Q." << product.utility << endl;
    }
    separator();
    cout << "Total ventas:\t\t\tQ." << month->totalSales << endl;
    cout << "Costos Variables:\t\tQ." << month->variableCosts << endl;
    cout << "Utilidades del mes:\t\tQ." << month->utility << endl;
}

/**
 * Thread function that analyzes a single product
 * @param args a structure of type Product to be casted
 * @return NULL, the values are stored in the same structure
 */
void* analyzeProductData(void *args){
    auto *product = (struct Product*) args;
    double price = product->baseProduct.price;
    double unitsSold = product->unitsSold;
    product->sales = unitsSold*price;
    product->utility = product->sales - (product->unitsSold * product->baseProduct.cost);
    pthread_exit(nullptr);
}