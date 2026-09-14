/**
 * 
 * FirmwareIterator
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
 * An iterator for the Firmware instances a {@link at.zimo.ulf.ZSU} file holds
 */
public class FirmwareIterator implements Iterator<Firmware>{
    /**
     * Construct a FirmwareIterator
     */
    FirmwareIterator(ZSU zsu) {
      _zsu = zsu;
    }

    /**
     * Checks, if the iterator has a next firmware
     * 
     * @return true, if there is a next firmware
     */
    @Override
    public boolean hasNext() {
      return index < count; 
    }

    /**
     * Return the next firmware
     * 
     * @return next firmware
     * 
     * @throws NoSuchElementException If out of bounds
     */
    @Override
    public Firmware next() {
      if (!hasNext()) {
        throw new NoSuchElementException();
      }
      return _zsu.getFirmware(index++);
    }
  
    private final ZSU _zsu; 
    private int index = 0; 
    private final int count = 0; 
}
