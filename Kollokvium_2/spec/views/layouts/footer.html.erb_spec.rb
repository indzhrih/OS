# frozen_string_literal: true

require 'rails_helper'

RSpec.describe 'layouts/partials/_footer.html.erb', type: :view do
  before { render partial: 'layouts/partials/footer' }

  it 'include year and app name' do
    expect(rendered).to have_content('2026 | Tasks')
  end

  it 'include app description' do
    expect(rendered).to have_content('Application for tasks management')
  end
end
