#include <bits/stdc++.h>
#include <iostream>
using namespace std;
using ll = long long;

// Modular exponentiation
ll modPow(ll base, ll exp, ll mod) {
    ll result = 1;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// Modular inverse using Extended Euclidean Algorithm
ll modInverse(ll a, ll m) {
    ll m0 = m, t, q;
    ll x0 = 0, x1 = 1;
    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    return (x1 < 0) ? x1 + m0 : x1;
}

// Check if a number is prime
bool isPrime(ll n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

// Check if the prime is among the first 5 weak primes
bool isWeakPrime(ll n) {
    static const unordered_set<ll> weakPrimes = {2, 3, 5, 7, 11};
    return weakPrimes.count(n) > 0;
}

// Simple hash function for demonstration
ll simpleHash(const string& message) {
    ll hash = 0;
    for (char ch : message)
        hash = (hash * 31 + ch) % 1000;
    return hash;
}

// Generate RSA keys
bool generateRSAKeyPair(ll p, ll q, ll &n, ll &e, ll &d) {
    if (!isPrime(p) || !isPrime(q) || p == q || isWeakPrime(p) || isWeakPrime(q)) {
        cout << "❌ Error: Both p and q must be distinct, strong prime numbers (not in the first 5 primes).\n";
        return false;
    }

    n = p * q;
    ll phi = (p - 1) * (q - 1);

    // Try commonly used public exponent first
    ll candidateE = 65537;
    if (__gcd(candidateE, phi) == 1 && candidateE < phi) {
        e = candidateE;
    } else {
        // Otherwise find the smallest valid e
        for (e = 3; e < phi; e += 2) {
            if (__gcd(e, phi) == 1) break;
        }
    }

    d = modInverse(e, phi);
    return true;
}

// Encrypt character
ll encryptChar(char ch, ll e, ll n) {
    return modPow(ch, e, n);
}

// Decrypt character
char decryptChar(ll encryptedChar, ll d, ll n) {
    return static_cast<char>(modPow(encryptedChar, d, n));
}

// Sign the message (sign the hash)
ll signMessage(const string& message, ll d, ll n) {
    ll hash = simpleHash(message);
    return modPow(hash, d, n);
}

// Verify the digital signature
bool verifySignature(const string& message, ll signature, ll e, ll n) {
    ll originalHash = simpleHash(message);
    ll decryptedHash = modPow(signature, e, n);
    return originalHash == decryptedHash;
}

int main() {
    ll p, q, n, e, d;

    // Input strong prime numbers
    while (true) {
        cout << "Enter two distinct strong prime numbers (p and q): ";
        cin >> p >> q;
        cin.ignore(); // Clear newline
        if (generateRSAKeyPair(p, q, n, e, d)) break;
        cout << "Please try again.\n";
    }

    cout << "\nGenerated Public Key: (n = " << n << ", e = " << e << ")\n";
    cout << "Generated Private Key: (n = " << n << ", d = " << d << ")\n";

    // Input message
    string message;
    cout << "\nEnter a message to encrypt and sign: ";
    getline(cin, message);

    // Encrypt
    vector<ll> encryptedMessage;
    for (char ch : message)
        encryptedMessage.push_back(encryptChar(ch, e, n));

    cout << "\nEncrypted message: ";
    for (ll val : encryptedMessage)
        cout << val << " ";
    cout << "\n";

    // Decrypt
    string decrypted;
    for (ll val : encryptedMessage)
        decrypted += decryptChar(val, d, n);

    cout << "Decrypted message: " << decrypted << "\n";

    // Sign and verify
    ll hash = simpleHash(message);
    ll signature = signMessage(message, d, n);
    ll decryptedHash = modPow(signature, e, n);

    cout << "Hash of message: " << hash << "\n";
    cout << "Digital Signature: " << signature << "\n";
    cout << "Decrypted Signature (hash): " << decryptedHash << "\n";

    bool valid = verifySignature(message, signature, e, n);
    cout << "Signature Verification: " << (valid ? "VALID ✅" : "INVALID ❌") << "\n";

    return 0;
}
