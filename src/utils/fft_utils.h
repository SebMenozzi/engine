#include <valarray>

#include "types.h"

namespace utils
{
    // Complex Array type
    typedef std::valarray<complex> complexarray;

    // Useful Links:
    // - https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.216.7588&rep=rep1&type=pdf
    // - https://sites.google.com/site/markdolanprogramming/cis-3223/fast-fourier-transform
    // - https://rosettacode.org/wiki/Fast_Fourier_transform#C++
    // - https://www.geeksforgeeks.org/iterative-fast-fourier-transformation-polynomial-multiplication/
    // - https://www.semanticscholar.org/paper/The-Hybrid-Architecture-Parallel-Fast-Fourier-Palmer/c6a7497677a953e097eb8caebf5cfffff4ba0ef5/figure/9

    // FFT transform recursive
    inline void fftRecursive(complexarray& values) 
    {
        size_t n = values.size();

        if (n <= 1)
            return;

        complexarray even = values[std::slice(0, n / 2, 2)];
        complexarray odd = values[std::slice(1, n / 2, 2)];

        fftRecursive(even);
        fftRecursive(odd);

        for (size_t k = 0; k < n / 2; ++k)
        {
            complex value = std::polar(1.0, -2 * M_PI * k / n) * odd[k];
            values[k] = even[k] + value;
            values[k + n / 2] = even[k] - value;
        }
    }

    inline size_t bitReverse(size_t x, size_t log2n)
    {
        size_t n = 0;
        
        for (size_t i = 0; i < log2n; ++i)
        {
            n <<= 1;
            n |= (x & 1);
            x >>= 1;
        }
        
        return n;
    }

    // FFT transform iterative
    inline void fftIterative(complexarray& values) 
    {
        size_t n = values.size();

        size_t log2n = log2(n);

        // Create a copy of values
        complexarray valuesCopy(n);
        std::copy(begin(values), end(values), begin(valuesCopy));

        for (size_t i = 0; i < n; ++i)
        {
            int rev = bitReverse(i, log2n);
            valuesCopy[i] = values[rev];
        }
        
        for (size_t s = 1; s <= log2n; ++s)
        {
            size_t m = 1 << s; // 2^s
            size_t m2 = m >> 1; // m / 2

            complex wm = std::polar(1.0, -M_PI / m2);
            complex w = std::polar(1.0, 0.0);

            for (size_t j = 0 ; j < m2 ; ++j)
            {
                for (size_t k = j; k < n; k += m)
                {
                    // Twiddle factor
                    complex t = w * valuesCopy[k + m2];
                    complex u = valuesCopy[k];

                    valuesCopy[k] = u + t;
                    valuesCopy[k + m2] = u - t;
                }

                w *= wm;
            }
        }

        // Change the address to the new array
        values = valuesCopy;
    }

    // Reverse FFT transform
    inline void ifft(
        complexarray& values,
        void (*fft)(complexarray&)
    )
    {
        // Conjugate the complex numbers
        values = values.apply(std::conj);
    
        fft(values);
    
        // Conjugate the complex numbers again
        values = values.apply(std::conj);
    
        // Scale the numbers
        values /= values.size();
    }
}