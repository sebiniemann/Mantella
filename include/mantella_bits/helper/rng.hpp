namespace mant {
  class Rng {
    public:
      Rng() = delete;
      Rng(const Rng&) = delete;
      Rng& operator=(const Rng&) = delete;

      inline static std::mt19937_64& getGenerator() ;

      inline static void setSeed(
          const unsigned int& seed) ;

      inline static void setRandomSeed() ;

      inline static unsigned int getSeed() ;

    protected:
      inline static unsigned int& seed_() ;
  };

  //
  // Implementation
  //

  inline std::mt19937_64& Rng::getGenerator()  {
    static std::mt19937_64 generator;
    return generator;
  }

  inline void Rng::setSeed(
      const unsigned int& seed)  {
   seed_() = seed;

    getGenerator().seed(seed_());
    arma::arma_rng::set_seed(seed_());

  }

  inline void Rng::setRandomSeed()  {
    arma::arma_rng::set_seed_random();
    setSeed(arma::randi<arma::Col<unsigned int>>(1).at(0));
  }

  inline unsigned int Rng::getSeed()  {
    return seed_();
  }

  inline unsigned int& Rng::seed_()  {
    static unsigned int seed;
    return seed;
  }
}
