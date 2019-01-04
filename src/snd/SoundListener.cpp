//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <snd/SoundListener.h>

#include <AL/al.h>
#include <AL/alc.h>

namespace mud
{
	SoundListener::SoundListener()
	{}

	void SoundListener::update()
	{
		if(m_transformUpdated)
		{
			alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
			alListenerfv(AL_ORIENTATION, value_ptr(m_front));

			//alListener3f(AL_VELOCITY, );

			m_transformUpdated = false;
		}     
	}
}
