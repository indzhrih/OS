# frozen_string_literal: true

require 'rails_helper'

RSpec.describe 'Pages routes', type: :routing do
  let(:user) { create(:user) }

  describe 'root path' do
    it 'routes root path' do
      expect(get: '/').to route_to(controller: 'pages', action: 'home')
    end
  end
end
