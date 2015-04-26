namespace mant {
  template <typename ParameterType>
  class Hash {
    public:
      inline std::size_t operator() (
          const arma::Col<ParameterType>& key) const noexcept;
    };
    
  template <typename ParameterType>
  class IsEqual {
    public:
      inline bool operator() (
          const arma::Col<ParameterType>& firstKey,
          const arma::Col<ParameterType>& secondKey) const;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  inline std::size_t Hash::operator() (
    const arma::Col<ParameterType>& key) const noexcept {
    // Start with the hash of the first value ...
    std::size_t hashedKey = std::hash<ParameterType>()(key(0));

    // ... and add the hash value of all following values to it.
    // Note: This is adapted from the Boost library (boost::hash_combine).
    for (const auto& value : key) {
      hashedKey ^= std::hash<ParameterType>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
    }

    return hashedKey;
  }
  
  template <typename ParameterType>
  inline bool IsEqual::operator() (
    const arma::Col<ParameterType>& firstKey,
    const arma::Col<ParameterType>& secondKey) const {
  // TODO Add exception
      
    return arma::all(firstKey == secondKey);
  }
}
