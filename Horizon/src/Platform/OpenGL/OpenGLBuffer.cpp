#include "hzpch.h"
#include "OpenGLBuffer.h"
#include <algorithm>

#include <glad/glad.h>

namespace Horizon {

	/////////////////																						//////////////
	// VertexBuffer //																						//////////////
	/////////////////																						//////////////

	

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		HZ_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		HZ_PROFILE_FUNCTION();	
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}


	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/////////////////																						//////////////
	// IndexBuffer //																						//////////////
	/////////////////																						//////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	

	OpenGLStorageBuffer::OpenGLStorageBuffer()
	{
		glGenBuffers(1, &m_RendererID);
		m_Size = 0;
		m_LayoutID = StorageBuffer::StorageLayoutCount;
		StorageBuffer::StorageLayoutCount++;
	}

	void OpenGLStorageBuffer::Bind() const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_LayoutID, m_RendererID);
	}

	void OpenGLStorageBuffer::Unbind() const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_LayoutID, 0);
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLStorageBuffer::SetData(const void* data, uint32_t size)
	{
		// If data is too big, resize it
		if (size > m_Size)
		{
			m_Size = std::max(size, m_Size * 2);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
			glBufferData(GL_SHADER_STORAGE_BUFFER, m_Size, nullptr, GL_DYNAMIC_DRAW);
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
	}

}