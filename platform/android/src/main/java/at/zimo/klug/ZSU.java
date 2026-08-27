package at.zimo.klug;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Represents a .zsu file loaded with the library. This essentially manages 
 * the underlying handle, but needs cleanup to avoid memory leaks.
 * 
 * @see cleanup
 */
public class ZSU implements Iterable<Firmware> {
  /**
   * Construct a ZSU from path
   * 
   * To use this on android, the file most likely needs to be located within 
   * the App sandbox, else the underlying syscalls may fail
   * 
   * @param path The path to the .zsu file (must be accessible with full rights)
   */
  public ZSU(String path) {
    _zsu = KLUGAdapter.zsuRead(path);
  }

  /**
   * Deletes the underlying handle
   * 
   * This must be called before the object is deconstructed. Failing to do 
   * so WILL lead to memory leaks.
   */
  public void cleanup() {
    validateState();
    KLUGAdapter.zsuRelease(_zsu);
    _zsu = 0;
  }

  /**
   * Creates a {@link at.zimo.klug.FirmwareIterator}.
   * 
   * @return iterator
   */
  @Override
  public Iterator<Firmware> iterator() {
    return new FirmwareIterator(this);
  }

  /**
   * Returns the firmware at index
   * 
   * @throws NoSuchElementException If the firmware doesn't exist
   * @throws IllegalStateException  If the object has been 
   *                                invalidated
   */
  public Firmware getFirmware(int index) {
    validateState();
    if (index >= KLUGAdapter.zsuGetFirmwareCount(_zsu)) {
      throw new NoSuchElementException(); 
    }
    return new Firmware(this, index);
  }

  /**
   * Get the underlying handle
   * 
   * @return handle
   * 
   * @throws IllegalStateException  If the object has been 
   *                                invalidated
   */
  long get() {
    validateState();
    return _zsu;
  }

  /**
   * Checks if the underlying handle still exists
   * 
   * @throws IllegalStateException  If the object has been 
   *                                invalidated
   */
  private void validateState() {
    if (_zsu == 0) throw new IllegalStateException("ZPP was closed");
  }

  private long _zsu; 
}
