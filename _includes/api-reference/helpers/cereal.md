
- Quellcode-Beispiel zum JSON-Format.
- Welche Type von Archiven werden unterstützt?

---
**<small>void</small> save( <small>T1</small> A, <small>arma::Col&lt;T2&gt;</small> D )** {% include noexcept %}

- Serialises a column vector *D* into an archive *A*.
- The serialisation (JSON, binary, ...) is defined by *A*.

---
**<small>void</small> load( <small>T1</small> A, <small>arma::Col&lt;T2&gt;</small> D )** {% include noexcept %}

- Deserialises an archive *A* into a column vector *D*.
- **Warning:** *D* should be empty or unused, as any previous data in *D* will be overwritten and its shape set to match *A*.

---
**<small>void</small> save( <small>T1</small> A, <small>arma::Row&lt;T2&gt;</small> D )** {% include noexcept %}

- Serialises a row vector *D* into an archive *A*.
- The serialisation (JSON, binary, ...) is defined by *A*.

---
**<small>void</small> load( <small>T1</small> A, <small>arma::Row&lt;T2&gt;</small> D )** {% include noexcept %}

- Deserialises an archive *A* into a row vector *D*.
- **Warning:** *D* should be empty or unused, as any previous data in *D* will be overwritten and its shape set to match *A*.

---
**<small>void</small> save( <small>T1</small> A, <small>arma::Mat&lt;T2&gt;</small> D )** {% include noexcept %}

- Serialises a matrix *D* into an archive *A*.
- The serialisation (JSON, binary, ...) is defined by *A*.

---
**<small>void</small> load( <small>T1</small> A, <small>arma::Mat&lt;T2&gt;</small> D )** {% include noexcept %}

- Deserialises an archive *A* into a matrix *D*.
- **Warning:** *D* should be empty or unused, as any previous data in *D* will be overwritten and its shape set to match *A*.