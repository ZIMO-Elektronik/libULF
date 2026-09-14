/**
 * 
 * Firmware
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

/**
 * Represents a single firmware within a {@link at.zimo.ulf.ZSU} file. Methods use the {@link [at.zimo.ulf.ULFAdapter]} 
 * API if applicable and an instance only holds constant identifiers of a firmware.
 */
public class Firmware {
  /**
   * Construct a firmware
   * 
   * @param zsu     The {@link at.zimo.ulf.ZSU} file this firmware is part of
   * @param index   The index within the {@link at.zimo.ulf.ZSU} file
   */
  Firmware(ZSU zsu, int index) {
    _zsu = zsu; 
    _index = index;
  }

  /**
   * Returns the firmware ID (matching the compatible decoder ID)
   * 
   * @return Firmware ID
   * 
   * @throws IllegalStateException If the {@link at.zimo.ulf.ZSU} was closed
   */
  public long id() {
    return ULFAdapter.zsuGetFirmwareId(_zsu.get(), _index);
  }

  /**
   * Returns the firmware name (matching the compatibel decoder name e.g. MS450)
   * 
   * @return Firmware Name
   * 
   * @throws IllegalStateException If the {@link at.zimo.ulf.ZSU} was closed
   */
  public String name() {
    return ULFAdapter.zsuGetFirmwareName(_zsu.get(), _index);
  }

  /**
   * Returns the bootloader type (Only needed for MX-Decoders)
   * 
   * @return Bootloader type
   * 
   * @throws IllegalStateException If the {@link at.zimo.ulf.ZSU} was closed
   */
  public int type() {
    return ULFAdapter.zsuGetFirmwareType(_zsu.get(), _index);
  }

  /**
   * Returns the number of flash blocks within the firmware
   * 
   * @return Flash block count
   * 
   * @throws IllegalStateException If the {@link at.zimo.ulf.ZSU} was closed
   */
  public int blocks() {
    return ULFAdapter.zsuGetFirmwareBlockCount(_zsu.get(), _index);
  }

  /**
   * Returns the firmware major version   
   * 
   * @return Major version
   *  
   * @throws IllegalStateException If the {@link at.zimo.ulf.ZSU} was closed
   */
  public String versionMajor() {
    return ULFAdapter.zsuGetFirmwareMajorVersion(_zsu.get(), _index);
  }

  /**
   * Returns the firmware minor version
   * 
   * @return Minor version
   * 
   * @throws IllegalStateException If the {@link at.zimo.ulf.ZSU} was closed
   */
  public String versionMinor() {
    return ULFAdapter.zsuGetFirmwareMinorVersion(_zsu.get(), _index);
  }

  /**
   * Returns the index of this firmware within the {@link at.zimo.ulf.ZSU} file
   * 
   * @return Index
   */
  public int index() {
    return _index;
  }

  /**
   * Returns the holding {@link at.zimo.ulf.ZSU} file
   * 
   * @return zsu
   */
  public ZSU zsu() {
    return _zsu;
  }

  private final ZSU _zsu; 
  private final int _index; 
}
