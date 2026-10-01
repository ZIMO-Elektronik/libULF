/**
 * 
 * ZPP
 * 
 * 08.09.2026
 * 
 * Copyright (C) 2026 ZIMO Elektronik
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
    _zpp = ULFAdapter.zppRead(path);
  }

  /**
   * Deletes the underlying handle
   * 
   * This must be called before the object is deconstructed. Failing to do 
   * so WILL lead to memory leaks.
   */
  public void cleanup() {
    validateState();
    ULFAdapter.zppRelease(_zpp);
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
    return ULFAdapter.zppBlocks(_zpp);
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
    return ULFAdapter.zppAuthor(_zpp);
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
    return ULFAdapter.zppEmail(_zpp);
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
