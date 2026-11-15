#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROWS 5
#define MAX_COLS 5

// Structure to define a section/category
struct Section {
    char name[20];
    float price;
    int rows;
    int cols;
    int seats[MAX_ROWS][MAX_COLS]; // 0 = Available, 1 = Booked
};

// Function to initialize venue data
void setupVenue(struct Section sec[], int n) {
    // Section 0: VIP
    strcpy(sec[0].name, "VIP");
    sec[0].price = 1500.0;
    sec[0].rows = 3;
    sec[0].cols = 4;

    // Section 1: Regular
    strcpy(sec[1].name, "Regular");
    sec[1].price = 600.0;
    sec[1].rows = 4;
    sec[1].cols = 5;

    // Set all seats to 0 (Available)
    for (int i = 0; i < n; i++) {
        for (int r = 0; r < sec[i].rows; r++) {
            for (int c = 0; c < sec[i].cols; c++) {
                sec[i].seats[r][c] = 0;
            }
        }
    }
}

// Function to show available seats as a seat map (Feature 2)
void displaySeatMap(struct Section sec[], int n) {
    printf("\n--- SEAT MAP (0: Available, X: Booked) ---\n");
    for (int i = 0; i < n; i++) {
        printf("\nSection: %s (Price: %.2f INR)\n", sec[i].name, sec[i].price);
        printf("   ");
        for (int c = 0; c < sec[i].cols; c++) {
            printf("C%d ", c + 1);
        }
        printf("\n");

        for (int r = 0; r < sec[i].rows; r++) {
            printf("R%d  ", r + 1);
            for (int c = 0; c < sec[i].cols; c++) {
                if (sec[i].seats[r][c] == 0)
                    printf("[ ] ");
                else
                    printf("[X] ");
            }
            printf("\n");
        }
    }
}

// Function to book selected seats and generate a ticket ID (Feature 3)
void bookTickets(struct Section sec[], int n) {
    int secChoice, row, col;
    
    printf("\n--- BOOK TICKETS ---\n");
    printf("Available Sections:\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s (Price: %.2f)\n", i + 1, sec[i].name, sec[i].price);
    }
    
    printf("Enter section number: ");
    scanf("%d", &secChoice);
    if (secChoice < 1 || secChoice > n) {
        printf("Invalid section choice!\n");
        return;
    }
    int idx = secChoice - 1;

    printf("Enter row (1 to %d): ", sec[idx].rows);
    scanf("%d", &row);
    printf("Enter column/seat (1 to %d): ", sec[idx].cols);
    scanf("%d", &col);

    if (row < 1 || row > sec[idx].rows || col < 1 || col > sec[idx].cols) {
        printf("Invalid seat position!\n");
        return;
    }

    // Adjust for 0-based indexing
    int r = row - 1;
    int c = col - 1;

    if (sec[idx].seats[r][c] == 1) {
        printf("Error: Seat is already booked! Overselling prevented.\n");
    } else {
        sec[idx].seats[r][c] = 1;
        int ticketId = rand() % 9000 + 1000; // Generate 4-digit random ticket ID
        printf("Booking Successful!\n");
        printf("-> Ticket ID: TKT-%d\n", ticketId);
        printf("-> Section: %s | Seat: Row %d, Col %d\n", sec[idx].name, row, col);
        printf("-> Total Amount to Pay: %.2f INR\n", sec[idx].price);
    }
}

// Function to cancel tickets with a refund (Feature 4)
void cancelTicket(struct Section sec[], int n) {
    int secChoice, row, col, daysLeft;

    printf("\n--- CANCEL TICKET & REFUND ---\n");
    printf("Enter section number (1 for VIP, 2 for Regular): ");
    scanf("%d", &secChoice);
    if (secChoice < 1 || secChoice > n) {
        printf("Invalid section!\n");
        return;
    }
    int idx = secChoice - 1;

    printf("Enter booked row: ");
    scanf("%d", &row);
    printf("Enter booked column: ");
    scanf("%d", &col);

    int r = row - 1;
    int c = col - 1;

    if (row < 1 || row > sec[idx].rows || col < 1 || col > sec[idx].cols) {
        printf("Invalid seat coordinates!\n");
        return;
    }

    if (sec[idx].seats[r][c] == 0) {
        printf("This seat is not currently booked.\n");
    } else {
        printf("Enter days left until the event: ");
        scanf("%d", &daysLeft);

        float refundRate = 0.0;
        if (daysLeft >= 10) {
            refundRate = 0.90; // 90% refund
        } else if (daysLeft >= 5) {
            refundRate = 0.70; // 70% refund
        } else {
            refundRate = 0.40; // 40% refund
        }

        float refundAmount = sec[idx].price * refundRate;
        sec[idx].seats[r][c] = 0; // Free up seat

        printf("Cancellation Successful!\n");
        printf("Refund Issued: %.2f INR (%.0f%% of ticket price)\n", refundAmount, refundRate * 100);
    }
}

// Function to show sales by section and total revenue (Feature 5)
void showSales(struct Section sec[], int n) {
    float totalRevenue = 0.0;

    printf("\n--- SALES & REVENUE REPORT ---\n");
    for (int i = 0; i < n; i++) {
        int bookedCount = 0;
        for (int r = 0; r < sec[i].rows; r++) {
            for (int c = 0; c < sec[i].cols; c++) {
                if (sec[i].seats[r][c] == 1) {
                    bookedCount++;
                }
            }
        }
        float sectionRevenue = bookedCount * sec[i].price;
        totalRevenue += sectionRevenue;

        printf("Section: %s\n", sec[i].name);
        printf("  - Booked Seats: %d / %d\n", bookedCount, sec[i].rows * sec[i].cols);
        printf("  - Revenue: %.2f INR\n", sectionRevenue);
    }
    printf("----------------------------------\n");
    printf("Total Event Revenue: %.2f INR\n", totalRevenue);
}

int main() {
    int numSections = 2;
    struct Section venue[2];

    setupVenue(venue, numSections);

    int choice;
    do {
        printf("\n=============================\n");
        printf("   TICKETVERSE - MAIN MENU   \n");
        printf("=============================\n");
        printf("1. View Venue Seat Map\n");
        printf("2. Book Seats\n");
        printf("3. Cancel Ticket / Refund\n");
        printf("4. Show Sales & Total Revenue\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displaySeatMap(venue, numSections);
                break;
            case 2:
                bookTickets(venue, numSections);
                break;
            case 3:
                cancelTicket(venue, numSections);
                break;
            case 4:
                showSales(venue, numSections);
                break;
            case 5:
                printf("Exiting TicketVerse. Thank you!\n");
                break;
            default:
                printf("Invalid choice! Please choose between 1-5.\n");
        }
    } while (choice != 5);

    return 0;
}