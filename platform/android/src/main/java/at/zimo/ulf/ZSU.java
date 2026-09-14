/**
 * 
 * ZSU
 * 
 * 08.09.2026
 * 
 * Copyright (C) 2026 [ZIMO Elektronik]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 */

package at.zimo.ulf;

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
    _zsu = ULFAdapter.zsuRead(path);
  }

  /**
   * Deletes the underlying handle
   * 
   * This must be called before the object is deconstructed. Failing to do 
   * so WILL lead to memory leaks.
   */
  public void cleanup() {
    validateState();
    ULFAdapter.zsuRelease(_zsu);
    _zsu = 0;
  }

  /**
   * Creates a {@link at.zimo.ulf.FirmwareIterator}.
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
    if (index >= ULFAdapter.zsuGetFirmwareCount(_zsu)) {
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
