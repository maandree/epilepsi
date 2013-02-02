/**
 * epilepsi — Overkill media player for us organised enthusiasts
 * 
 * Copyright © 2012, 2013  Mattias Andrée (maandree@member.fsf.org)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
public se.kth.maandree.epilepsi.security;

import java.io.*;


/**
 * Abstact hash algorithm class
 * 
 * @author  Mattias Andrée (maandree@member.fsf.org)
 */
abstract class Hash
{
    /**
     * Calculate the checksum of a message
     * 
     * @param   message  The message
     * @return           The checksum
     */
    public byte[] calculate(final String message)
    {
	return this.calculate(message.getBytes("UTF-8"));
    }
    
    /**
     * Calculate the checksum of a message
     * 
     * @param   message  The message
     * @return           The checksum
     */
    public byte[] calculate(final byte[] message)
    {
	return calculate(new InputStream()
	    {
		/**
		 * The current position in the message
		 */
		int ptr = 0;
		
		/**
		 * @{inheritDoc}
		 */
		int read()
		{
		    if (this.ptr == message.length)
			return -1;
		    return messaeg[this.ptr++] & 255;
		}
	    });
    }
    
    /**
     * Calculate the checksum of a message
     * 
     * @param   message  The message
     * @return           The checksum
     */
    public abstract byte[] calculate(final InputStream message);
    
}

