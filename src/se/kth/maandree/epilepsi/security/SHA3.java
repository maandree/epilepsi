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
package se.kth.maandree.epilepsi.security;

import java.io.*;
import java.security.*;


/**
 * SHA3 hash algorithm class
 * 
 * @author  Mattias Andrée (maandree@member.fsf.org)
 */
class SHA3 extends Hash
{
    /**
     * Buffer size
     */
    private final int BUF_SIZE = 1024;
    
    
    
    /**
     * Constructor
     * 
     * @param  bits  The bit size
     */
    public SHA3(final int bits)
    {
    }
    
    
    
    /**
     * @{inheritDoc}
     */
    public byte[] calculate(final InputStream message) throws IOException
    {
	for (int n;;)
	    if ((n = message.read(this.buf, 0, BUF_SIZE)) > 0)
		this.digest.update(this.buf, 0, n);
	    else
		return this.digest.digest();
    }
    
}

