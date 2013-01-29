# CircBuffer

CircBuffer is an Arduino library implementing a single-reader circular
buffer. The library allows a single writer to insert data, with a
single reader extracting data from the same shared buffer. The
implementation does not use malloc(), instead the caller must provide
the buffer space and length when creating the CircBuffer object. Any
buffer size between 1 and max int -1 is valid. For a more complete
implementation featuring multiple readers and optional overwrite
protection see https://github.com/stevemarple/MultiReadCircBuffer

