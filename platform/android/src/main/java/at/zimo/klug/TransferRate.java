/**
 * 
 * TransferRate
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

package at.zimo.klug;

/**
 * MDU Transfer rate enum
 */
public enum TransferRate {
  Fallback(0), 
  Fast(1),
  Medium(2),
  Slow(3), 
  Default(4);
  
  TransferRate(int value) {
    if (value < 0 || value > 4) throw new IllegalArgumentException("Value must be between 0 and 4");
    this._value = value; 
  }
  
  public int value() {
    return _value; 
  }
  private final int _value;
}