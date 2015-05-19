namespace mant {
  template <typename T>
  class Hash {
    public:
      inline std::size_t operator() (
          const arma::Col<T>& key) const noexcept;
    };
    
  template <typename T>
  class IsEqual {
    public:
      inline bool operator() (
          const arma::Col<T>& firstKey,
          const arma::Col<T>& secondKey) const;
  };

  //
  // Implementation
  //

  template <typename T>
  inline std::size_t Hash<T>::operator() (
    const arma::Col<T>& key) const noexcept {
    // Start with the hash of the first value ...
    std::size_t hashedKey = std::hash<T>()(key(0));

    // ... and add the hash value of all following values to it.
    // Note: This is adapted from the Boost library (boost::hash_combine).
    for (const auto& value : key) {
      hashedKey ^= std::hash<T>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
    }

    return hashedKey;
  }
  
  template <typename T>
  inline bool IsEqual<T>::operator() (
    const arma::Col<T>& firstKey,
    const arma::Col<T>& secondKey) const {
  // TODO Add exception
      
    return arma::all(firstKey == secondKey);
  }
}
