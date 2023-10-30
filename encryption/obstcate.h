#ifndef CRYPTOSYSTEM_HPP
#define CRYPTOSYSTEM_HPP

#include <array>

#ifndef META_RANDOM_HPP
#define META_RANDOM_HPP

#include <limits>

namespace snowapril {

	constexpr int DigitToInt(char c) {
		return c - '0';
	};

	constexpr int RandomSeed(void) {
		constexpr char time[] = __TIME__;

		constexpr int seed = DigitToInt(time[7]) +
			DigitToInt(time[6]) * 10 +
			DigitToInt(time[4]) * 60 +
			DigitToInt(time[3]) * 600 +
			DigitToInt(time[1]) * 3600 +
			DigitToInt(time[0]) * 36000;

		return seed;
	};

	template <unsigned int a,
		unsigned int c,
		unsigned int seed,
		unsigned int Limit>
		struct LinearCongruentialEngine {
		enum { value = (a * LinearCongruentialEngine<a, c - 1, seed, Limit>::value + c) % Limit };
	};

	template <unsigned int a,
		unsigned int seed,
		unsigned int Limit>
		struct LinearCongruentialEngine<a, 0, seed, Limit> {
		enum { value = (a * seed) % Limit };
	};

	template <int N, int Limit>
	struct MetaRandom {
		enum { value = LinearCongruentialEngine<16807, N, RandomSeed(), Limit>::value };
	};

}

#endif


namespace snowapril {

	template <int A, int B>
	struct ExtendedEuclidian {
		enum {
			d = ExtendedEuclidian<B, A% B>::d,
			x = ExtendedEuclidian<B, A% B>::y,
			y = ExtendedEuclidian<B, A% B>::x - (A / B) * ExtendedEuclidian<B, A% B>::y
		};
	};

	template <int A>
	struct ExtendedEuclidian<A, 0> {
		enum {
			d = A,
			x = 1,
			y = 0
		};
	};

	constexpr std::array<int, 30> PrimeNumbers = {
		2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
		31, 37, 41, 43, 47, 53, 59, 61, 67,
		71, 73, 79, 83, 89, 97, 101, 103,
		107, 109, 113
	};

	constexpr int positive_modulo(int a, int n) {
		return (a % n + n) % n;
	}

	template <typename Indexes, int A, int B>
	class MetaString;

	template <unsigned int... I, int A, int B>
	class MetaString<std::index_sequence<I...>, A, B> {
	public:
		constexpr MetaString(const char* str)
			: encrypted_buffer{ encrypt(str[I])... } {};
	public:
		inline const char* decrypt(void) {
			for (size_t i = 0; i < sizeof...(I); ++i) {
				buffer[i] = decrypt(encrypted_buffer[i]);
			}
			buffer[sizeof...(I)] = 0;
			return buffer;
		}
	private:
		constexpr int encrypt(char c) { return (A * (c)+B) % 127; };
		constexpr char decrypt(int c) { return positive_modulo((ExtendedEuclidian<127, A>::y) * (c - B), 127); };
	private:
		char buffer[sizeof...(I) + 1];
		int  encrypted_buffer[sizeof...(I)];
	};
}

#define OBFUSCATE(str) (snowapril::MetaString<std::make_index_sequence<sizeof(str) - 1>, \
					      std::get<snowapril::MetaRandom<__COUNTER__, 30>::value>(snowapril::PrimeNumbers), \
					      snowapril::MetaRandom<__COUNTER__, 126>::value>(str).decrypt())

#endif