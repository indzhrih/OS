# frozen_string_literal: true

require 'rails_helper'

RSpec.describe 'pages/home.html.erb', type: :view do
  before { render template: 'pages/home' }

  it "include 'TASKS' label" do
    expect(rendered).to have_content('TASKS')
  end

  it 'include calling label' do
    expect(rendered).to have_content('Start managing your tasks right now!')
  end

  it 'include start button' do
    expect(rendered).to have_content('START NOW')
  end
end
