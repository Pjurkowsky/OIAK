#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

struct Vector
{
    float x, y, z, w;
};

// populates vectors with random numbers
void populateVectors(Vector *vectors, int numberOfVectors)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(1, 101);

    for (int i = 0; i < numberOfVectors; i++)
    {
        vectors[i].x = dist(mt);
        vectors[i].y = dist(mt);
        vectors[i].z = dist(mt);
        vectors[i].w = dist(mt);
    }
}

// populates arrayss with random numbers
void populateArrays(float *array1, int numberOfNumbers)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(1, 101);

    for (int i = 0; i < numberOfNumbers; i++)
        array1[i] = dist(mt);
}

// add two given by arguments vectors using assembly
double addVectorSIMD(Vector *v1, Vector *v2, Vector *result)
{
    auto start = std::chrono::high_resolution_clock::now();
    __asm__ __volatile__(
        "movups (%0), %%xmm0\n"
        "movups (%1), %%xmm1\n"
        "addps %%xmm1, %%xmm0\n"
        "movups %%xmm0, (%2)\n"
        :
        : "r"(v1), "r"(v2), "r"(result)
        : "%xmm0", "%xmm1");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

// sub two given by arguments vectors using assembly
double subVectorSIMD(Vector *v1, Vector *v2, Vector *result)
{
    auto start = std::chrono::high_resolution_clock::now();
    __asm__ __volatile__(
        "movups (%0), %%xmm0\n"
        "movups (%1), %%xmm1\n"
        "subps %%xmm1, %%xmm0\n"
        "movups %%xmm0, (%2)\n"
        :
        : "r"(v1), "r"(v2), "r"(result)
        : "%xmm0", "%xmm1");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

// mul two given by arguments vectors using assembly
double mulVectorSIMD(Vector *v1, Vector *v2, Vector *result)
{
    auto start = std::chrono::high_resolution_clock::now();
    __asm__ __volatile__(
        "movups (%0), %%xmm0\n"
        "movups (%1), %%xmm1\n"
        "mulps %%xmm1, %%xmm0\n"
        "movups %%xmm0, (%2)\n"
        :
        : "r"(v1), "r"(v2), "r"(result)
        : "%xmm0", "%xmm1");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

// div two given by arguments vectors using assembly
double divVectorSIMD(Vector *v1, Vector *v2, Vector *result)
{
    auto start = std::chrono::high_resolution_clock::now();
    __asm__ __volatile__(
        "movups (%0), %%xmm0\n"
        "movups (%1), %%xmm1\n"
        "divps %%xmm1, %%xmm0\n"
        "movups %%xmm0, (%2)\n"
        :
        : "r"(v1), "r"(v2), "r"(result)
        : "%xmm0", "%xmm1");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

// add two given numbers using assembly
double addSISD(float *x, float *y, float *result)
{
    auto start = std::chrono::high_resolution_clock::now();
    __asm__ __volatile__(
        "flds (%0)\n"
        "fadds (%1)\n"
        "fstps (%2)\n"
        :
        : "r"(x), "r"(y), "r"(result)
        : "%eax");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

// sub two given numbers using assembly
double subSISD(float *x, float *y, float *result)
{
    auto start = std::chrono::high_resolution_clock::now();
    __asm__ __volatile__(
        "flds (%0)\n"
        "fsubs (%1)\n"
        "fstps (%2)\n"
        :
        : "r"(x), "r"(y), "r"(result)
        : "%eax");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

// mul two given numbers using assembly
double mulSISD(float *x, float *y, float *result)
{
    auto start = std::chrono::high_resolution_clock::now();
    __asm__ __volatile__(
        "flds (%0)\n"
        "fmuls (%1)\n"
        "fstps (%2)\n"
        :
        : "r"(x), "r"(y), "r"(result)
        : "%eax");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

//  div two given numbers using assembly
double divSISD(float *x, float *y, float *result)
{
    auto start = std::chrono::high_resolution_clock::now();
    __asm__ __volatile__(
        "flds (%0)\n"
        "fdivs (%1)\n"
        "fstps (%2)\n"
        :
        : "r"(x), "r"(y), "r"(result)
        : "%eax", "%ebx", "%edx");

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

// tests all operations on vectors and saves time
void testSIMD(int numberOfNumbers, int numberOfTests, double *time)
{
    Vector *v1 = new Vector[numberOfNumbers];
    Vector *v2 = new Vector[numberOfNumbers];
    Vector *result = new Vector[numberOfNumbers];

    populateVectors(v1, numberOfNumbers);
    populateVectors(v2, numberOfNumbers);

    for (int i = 0; i < numberOfTests; i++)
    {
        for (int j = 0; j < numberOfNumbers; j++)
        {
            time[0] += addVectorSIMD(&v1[j], &v2[j], &result[j]);
            time[1] += subVectorSIMD(&v1[j], &v2[j], &result[j]);
            time[2] += mulVectorSIMD(&v1[j], &v2[j], &result[j]);
            time[3] += divVectorSIMD(&v1[j], &v2[j], &result[j]);
        }
    }

    delete[] v1;
    delete[] v2;
    delete[] result;
}

// tests all operations on arrays and saves time
void testSISD(int numberOfNumbers, int numberOfTests, double *time)
{
    float *v1 = new float[numberOfNumbers];
    float *v2 = new float[numberOfNumbers];
    float *result = new float[numberOfNumbers];

    populateArrays(v1, numberOfNumbers);
    populateArrays(v2, numberOfNumbers);

    for (int i = 0; i < numberOfTests; i++)
    {
        for (int j = 0; j < numberOfNumbers; j++)
        {
            time[0] += addSISD(&v1[j], &v2[j], &result[j]);
            time[1] += subSISD(&v1[j], &v2[j], &result[j]);
            time[2] += mulSISD(&v1[j], &v2[j], &result[j]);
            time[3] += divSISD(&v1[j], &v2[j], &result[j]);
        }
    }

    delete[] v1;
    delete[] v2;
}

// saves time results to file
void saveTimeResults(double *time, int numberOfNumbers, int numberOfTests, std::string fileName)
{
    std::ofstream file(fileName);
    if (!file.is_open())
        std::cout << "Nie mozna otworzyc pliku!"
                  << "\n";

    file << "Typ obliczen: SIMD / SISD \n"
         << "Liczba liczb: " << numberOfNumbers << "\n"
         << "Sredni czas [x]: \n"
         << "+ " << time[0] / numberOfTests << "\n"
         << "- " << time[1] / numberOfTests << "\n"
         << "* " << time[2] / numberOfTests << "\n"
         << "/ " << time[3] / numberOfTests << "\n";
}

int main(int argc, char *argv[])
{
    double timeSIMD[4] = {}; // 0 - add, 1 - sub, 2 - mul, 3 - div
    double timeSISD[4] = {}; // 0 - add, 1 - sub, 2 - mul, 3 - div

    int numberOfNumbers, numberOfTests;
    switch (argc)
    {
    case 1:
        numberOfNumbers = 2048;
        numberOfTests = 10;
        break;
    case 2:
        numberOfNumbers = atoi(argv[1]);
        numberOfTests = 10;
        break;
    case 3:
        numberOfNumbers = atoi(argv[1]);
        numberOfTests = atoi(argv[2]);
        break;
    default:
        std::cout << "Zla liczba argumentow! \n";
        return 1;
    }

    // Vector v1 = {2.1, 2, 3, 4};
    // Vector v2 = {5, 6.2, 7, 8};
    // Vector resulte;

    // std::cout << addVectorSIMD(&v1, &v2, &resulte) << " " << resulte.x << " " << resulte.y << " " << resulte.z << " " << resulte.w << "\n";
    // std::cout << subVectorSIMD(&v1, &v2, &resulte) << " " << resulte.x << " " << resulte.y << " " << resulte.z << " " << resulte.w << "\n";
    // std::cout << mulVectorSIMD(&v1, &v2, &resulte) << " " << resulte.x << " " << resulte.y << " " << resulte.z << " " << resulte.w << "\n";
    // std::cout << divVectorSIMD(&v1, &v2, &resulte) << " " << resulte.x << " " << resulte.y << " " << resulte.z << " " << resulte.w << "\n";

    // float x = 222.22;
    // float y = 2;
    // float results = 0;
    // std::cout << addSISD(&x, &y, &results) << " " << results << "\n";
    // std::cout << subSISD(&x, &y, &results) << " " << results << "\n";
    // std::cout << mulSISD(&x, &y, &results) << " " << results << "\n";
    // std::cout << divSISD(&x, &y, &results) << " " << results << "\n";

    testSIMD(numberOfNumbers, numberOfTests, timeSIMD);
    testSISD(numberOfNumbers, numberOfTests, timeSISD);

    saveTimeResults(timeSIMD, numberOfNumbers, numberOfTests, "outputSIMD.txt");
    saveTimeResults(timeSISD, numberOfNumbers, numberOfTests, "outputSISD.txt");

    return 0;
}