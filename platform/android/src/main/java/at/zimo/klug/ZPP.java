package at.zimo.klug;

/**
 * Represents a .zpp file loaded with the library. This essentially manages 
 * the underlying handle, but needs cleanup to avoid memory leaks.
 * 
 * @see cleanup
 */
public class ZPP {
  /**
   * Construct a ZPP from path
   * 
   * To use this on android, the file most likely needs to be located within 
   * the App sandbox, else the underlying syscalls may fail
   * 
   * @param path The path to the .zpp file (must be accessible with full rights)
   */
  public ZPP(String path) {
    _zpp = KLUGAdapter.zppRead(path);
  }

  /**
   * Deletes the underlying handle
   * 
   * This must be called before the object is deconstructed. Failing to do 
   * so WILL lead to memory leaks.
   */
  public void cleanup() {
    validateState();
    KLUGAdapter.zppRelease(_zpp);
    _zpp = 0;
  }

  /**
   * Returns the number of flash blocks within the project
   * 
   * @return block count
   * 
   * @throws IllegalStateException  If the object has been 
   *                                invalidated
   */
  public int blocks() {
    validateState();
    return KLUGAdapter.zppBlocks(_zpp);
  }

  /**
   * Returns the author of the project
   * 
   * @return author
   * 
   * @throws IllegalStateException  If the object has been 
   *                                invalidated
   */
  public String author() {
    validateState();
    return KLUGAdapter.zppAuthor(_zpp);
  }

  /**
   * Returns the email of the project author
   * 
   * @return email
   * 
   * @throws IllegalStateException  If the object has been 
   *                                invalidated
   */
  public String email() {
    validateState();
    return KLUGAdapter.zppEmail(_zpp);
  }

  /**
   * Returns the underlying handle
   * 
   * @return handle
   * 
   * @throws IllegalStateException  If the object has been 
   *                                invalidated
   */
  long get() {
    validateState();
    return _zpp;
  }

  /**
   * Checks if the underlying handle still exists
   * 
   * @throws IllegalStateException  If the object has been 
   *                                invalidated
   */
  private void validateState() {
    if (_zpp == 0) throw new IllegalStateException("ZPP was closed");
  }

  private long _zpp;
}
