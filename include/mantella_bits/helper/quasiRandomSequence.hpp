namespace mant {
   inline arma::Mat<double> getHaltonSequence(
       const arma::Col<unsigned int>& base,
       const arma::Col<unsigned int>& seed,
       const unsigned int numberOfColumms);

   inline arma::Col<double> getVanDerCorputSequence(
       const unsigned int base,
       const unsigned int seed,
       const unsigned int numberOfColumms) noexcept;

   //
   // Implementation
   //

   inline arma::Mat<double> getHaltonSequence(
       const arma::Col<unsigned int>& base,
       const arma::Col<unsigned int>& seed,
       const unsigned int numberOfColumms) {
     verify(base.n_elem == seed.n_elem, "The number of dimensions of the base parameter must match the number of dimensions of the seed parameter.");

     arma::Mat<double> sequence(seed.n_elem, numberOfColumms);

     for (std::size_t n = 0; n < seed.n_elem; ++n) {
       sequence.row(n) = getVanDerCorputSequence(base(n), seed(n), numberOfColumms).t();
     }

     return sequence;
   }

   inline arma::Col<double> getVanDerCorputSequence(
       const unsigned int base,
       const unsigned int seed,
       const unsigned int numberOfColumms) noexcept {
     arma::Col<double> sequence(numberOfColumms);

     for (std::size_t n = 0; n < numberOfColumms; ++n) {
       double inverse = 1.0 / static_cast<double>(base);
       double number = 0.0;

       unsigned int remaining = seed + n;
       while (remaining > 0) {
         unsigned int digit = remaining % base;
         number = number + static_cast<double>(digit) * inverse;
         inverse = inverse / static_cast<double>(base);
         remaining = std::floor(remaining / base);
       }

       sequence(n) = number;
     }

     return sequence;
   }
}
