#include "pch.h"
#include "PostProcessChain.h"

PostProcessChain::PostProcessChain(PostProcessChain&& other) noexcept {
	m_passes = std::move(other.m_passes);
}
PostProcessChain& PostProcessChain::operator=(PostProcessChain&& other) noexcept {
	if (this != &other) {
		m_passes = std::move(other.m_passes);
	}
	return *this;
}
void PostProcessChain::allocate(size_t size) {
	m_passes.reserve(size);
}
void PostProcessChain::add(PostProcessPass& pass) {
	if (!m_passes.empty()) {
		if (m_passes.back() == &pass) {
			spdlog::error("The same PostProcessPass class appears consecutively.");
			return;
		}
	}
	m_passes.emplace_back(&pass);
}
void PostProcessChain::resize(int width, int height) {
	for (auto& rt : m_passes)
		rt->resize(width, height);
}
void PostProcessChain::clear() {
	m_passes.clear();
}
const GLTexture2D& PostProcessChain::execute(const GLTexture2D& texture, const GLMesh& screen) {
	//やりたい事的に参照ではなくポインタ使う
	const GLTexture2D* current = &texture;
	unsigned int procCount = 0;
	for (auto& rt : m_passes) {
		rt->shader().setUniformUInt("uProcCount", procCount);
		rt->execute(*current, screen);
		//出力されたテクスチャを次に渡すテクスチャとする
		current = &rt->output();
		procCount++;
	}
	return *current;
}