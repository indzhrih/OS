# frozen_string_literal: true

require 'rails_helper'

RSpec.describe PagesController, type: :controller do
  describe 'GET home' do
    before { get :home }

    it 'returns a 200' do
      expect(response).to have_http_status(:ok)
    end

    it 'renders the home template' do
      expect(response).to render_template('home')
    end
  end
end
